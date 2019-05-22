#pragma once
#include <Python.h>
#include <vector>
using namespace std;


PyMODINIT_FUNC PyInit_myMatrix(void);
static PyObject* myMatr_transpose(myMatrixObject* self);
static void myMatrix_dealloc(myMatrixObject* o);
static PyObject* matr_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
static int matr_init(myMatrixObject* self, PyObject* args);
static PyObject* matr_repr(myMatrixObject* self);
static Py_ssize_t matrix_length(myMatrixObject* self);
static PyObject* myMatr_transpose(myMatrixObject* self);
static PyObject* myMatr_add(myMatrixObject* self, PyObject* args);
static PyObject* myMatr_mul(myMatrixObject* self, PyObject* args);
static PyObject* myMatr_add_val(myMatrixObject* m, PyObject* args);
static PyObject* myMatr_div_val(myMatrixObject* self, PyObject* args);
static int matr_contains(myMatrixObject* self, PyObject* arg);
static PyObject* get_by_coordinates(myMatrixObject* self, PyObject* arg);
static PyObject* matr_str(myMatrixObject* self);