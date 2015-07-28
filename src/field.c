
#include "field.h"
#include "util.h"
#include <stdlib.h>

struct field *
field_init(struct field *field, int w, int h) {
  int num_vecs;

  num_vecs = w*h;
  field->vecs = (struct field_vec*) malloc(sizeof(struct field_vec) * num_vecs);
  field->num_vecs = num_vecs;

  return field;
}

void
field_free(struct field *field) {
  free(field->vecs);
}

int
field_add_global_force(struct field * field, struct field_vec force) {
  if ((size_t)field->num_global_forces > ARRAY_SIZE(field->global_forces)) {
    return -1;
  }
  field->global_forces[field->num_global_forces++] = force;
  return field->num_global_forces - 1;
}
