#ifndef SANDY_FIELD_H
#define SANDY_FIELD_H

struct field_vec {
  signed char x;
  signed char y;
};

struct field {
  struct field_vec global_forces[8];
  int num_global_forces;

  struct field_vec *vecs;
  int num_vecs;
};

struct field *
field_init(struct field *field, int w, int h);

int
field_add_global_force(struct field *field, struct field_vec force);

void
field_free(struct field *field);

#endif /* SANDY_FIELD_H */
