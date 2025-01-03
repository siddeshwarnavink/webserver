/* vi:set ts=2 sts=2 sw=2 et:
 *
 * template.h - HTML Template rendering engine
 *
 * Part of webserver project
 * by Siddeshwar <siddeshwar.work@gmail.com>
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H

char *render_template(const char *template_path, const char *placeholders[], const char *values[], size_t count);

#endif
