#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "template.h"
#include "log.h"

static char *_read_file(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    perror("Failed to open template file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *content = (char *)malloc(length + 1);
  if (!content) {
    perror("Failed to allocate memory for template content");
    fclose(file);
    return NULL;
  }

  fread(content, 1, length, file);
  content[length] = '\0';
  fclose(file);
  return content;
}

char *render_template(const char *path, const char *placeholders[], const char *values[], size_t count) {
  char full_path[256];
  snprintf(full_path, sizeof(full_path), "views/%s", path);

  LOG("Rendering template: %s\n", full_path);

  char *template = _read_file(full_path);
  if (!template) {
    LOG("Failed to read template file\n");
    return NULL;
  }

  char *rendered = (char *)malloc(strlen(template) + 1);
  if (!rendered) {
    LOG("Failed to allocate memory for rendered template\n");
    free(template);
    return NULL;
  }

  size_t rendered_index = 0;
  size_t template_index = 0;
  while (template[template_index] != '\0') {
    if (template[template_index] == '{' && template[template_index + 1] == '{') {
      template_index += 2;

      size_t placeholder_end = template_index;
      while (template[placeholder_end] != '\0' && template[placeholder_end] != '}') {
        placeholder_end++;
      }

      if (template[placeholder_end] == '\0') {
        LOG("Invalid placeholder in template\n");
        free(template);
        free(rendered);
        return NULL;
      }

      char placeholder[placeholder_end - template_index + 1];
      strncpy(placeholder, template + template_index, placeholder_end - template_index);
      placeholder[placeholder_end - template_index] = '\0';

      const char *value = NULL;
      for (size_t i = 0; i < count; i++) {
        if (strcmp(placeholder, placeholders[i]) == 0) {
          value = values[i];
          break;
        }
      }

      if (!value) {
        LOG("No value found for placeholder: %s\n", placeholder);
        free(template);
        free(rendered);
        return NULL;
      }

      size_t value_length = strlen(value);
      strncpy(rendered + rendered_index, value, value_length);
      rendered_index += value_length;

      template_index = placeholder_end + 2;
    } else {
      rendered[rendered_index] = template[template_index];
      rendered_index++;
      template_index++;
    }
  }

  rendered[rendered_index] = '\0';

  LOG("Rendered template: %s\n", rendered);

  free(template);
  return rendered;
}
