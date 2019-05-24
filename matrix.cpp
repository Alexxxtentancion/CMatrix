#include <Python.h>
#include<stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Header.h"
using namespace std;




static PyMethodDef matr_methods[] = {
	{ "add", (PyCFunction)myMatr_add, METH_VARARGS, "Method" },
	{"transpose",(PyCFunction)myMatr_transpose,METH_NOARGS,"Transposes matrix"},
	{ "matr_mul", (PyCFunction)myMatr_mul, METH_VARARGS, "Method" },
	{ "add_value", (PyCFunction)myMatr_add_val, METH_VARARGS, "Method" },
	{ "div", (PyCFunction)myMatr_div_val, METH_VARARGS, "Method" },
	{ NULL, NULL, 0, NULL }
};
static PySequenceMethods matrix_as_sequence = {
	(lenfunc)matrix_length,                            /* sq_length */
	0,                                               /* sq_concat */
	0,                                               /* sq_repeat */
	0,                                               /* sq_item */
	0,                                               /* sq_slice */
	0,                                               /* sq_ass_item */
	0,                                               /* sq_ass_slice */
	(objobjproc)matr_contains,                       /* sq_contains */
};
static PyMappingMethods matrix_as_mapping = {
	0,                                              /*mp_lenfunc*/
	(binaryfunc)get_by_coordinates,					/*mp_subscript*/
	0,												/*mp_ass_subscript*/

};

PyTypeObject matr_Type = {
	PyObject_HEAD_INIT(NULL)
	"myMatrix.Matrix",                               /* tp_name */
	sizeof(myMatrixObject),                          /* tp_basic_size */
	0,                                               /* tp_itemsize */
	(destructor)myMatrix_dealloc,                    /* tp_dealloc */
	0,                                               /* tp_print */
	0,                                               /* tp_getattr */
	0,                                               /* tp_setattr */
	0,                                               /* tp_reserved */
	reprfunc(matr_repr),                             /* tp_repr */
	0,                                               /* tp_as_number */
	&matrix_as_sequence,                             /* tp_as_sequence */
	&matrix_as_mapping,                              /* tp_as_mapping */
	PyObject_HashNotImplemented,                     /* tp_hash */
	0,                                               /* tp_call */
	reprfunc(matr_str),                              /* tp_str */
	0,                                               /* tp_getattro */
	0,                                               /* tp_setattro */
	0,                                               /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,        /* tp_flags */
	0,                                               /* tp_doc */
	0,												 /* tp_traverse */
	0,                                               /* tp_clear */
	0,                                               /* tp_richcompare */
	0,                                               /* tp_weaklistoffset */
	0,                                               /* tp_iter */
	0,                                               /* tp_iternext */
	matr_methods,                                    /* tp_methods */
	0,                                               /* tp_members */
	0,                                               /* tp_getset */
	0,                                               /* tp_base */
	0,                                               /* tp_dict */
	0,                                               /* tp_descr_get */
	0,                                               /* tp_descr_set */
	0,                                               /* tp_dictoffset */
	(initproc)matr_init,                             /* tp_init */
	0,                                               /* tp_alloc */
	matr_new,                                        /* tp_new */
	0,                                               /* tp_free */
};
static void myMatrix_dealloc(myMatrixObject* o)
{
	Py_TYPE(o)->tp_free((PyObject*)o);
}


static PyObject* matr_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
	myMatrixObject* self;

	self = (myMatrixObject*)type->tp_alloc(type, 0);
	if (self == NULL) {
		return PyErr_NoMemory();
	}

	self->v =vector<vector<float>>();
	return (PyObject*)self;
}

static int matr_init(myMatrixObject* self, PyObject* args)
{
	PyObject* m;
	PyObject* l;
	PyObject* li;
	Py_ssize_t nm, nl;
	Py_ssize_t size_check = 0;
	if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &m))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
		return -1;
	}
	nm = PyList_Size(m);
	for (int i = 0; i < nm; i++)
	{
		l = PyList_GetItem(m, i);
		Py_INCREF(l);
		if (PyList_Check(l))
		{
			nl = PyList_Size(l);

			if (nl == 0) {
				PyErr_SetString(PyExc_IndexError, "Empty list");
				return -1;
			}

			if (size_check == 0) {
				size_check = nl;
			}
			else {
				if (nl != size_check) {
					PyErr_SetString(PyExc_TypeError, "matrix must have fixed els count on every row");
					return -1;
				}
			}

			for (int j = 0; j < nl; j++)
			{
				li = PyList_GetItem(l, j);
				Py_INCREF(li);
				if (!PyLong_Check(li) && !PyFloat_Check(li))
				{
					PyErr_SetString(PyExc_TypeError, "list contains only integers as float values.");
					return -1;
				}
				Py_DECREF(li);
			}
		}
		else
		{
			PyErr_SetString(PyExc_TypeError, "argument must be a list.");
			return -1;
		}
		Py_DECREF(l);
	}
	self->v.clear();
	self->v.resize(nm, vector<float>(nl));
	for (int i = 0; i < nm; i++)
	{
		l = PyList_GetItem(m, i);
		for (int j = 0; j < nl; j++)
		{
			li = PyList_GetItem(l, j);
			self->v[i][j] = PyFloat_AsDouble(li);
		}
	}
	return 0;
}
static PyObject* get_by_coordinates(myMatrixObject* self, PyObject* arg)
{
	PyObject* i;
	PyObject* j;
	if (!PyTuple_Check(arg))
	{
		PyErr_SetString(PyExc_TypeError, "argument must be a tuple");
	}
	i = PyTuple_GetItem(arg, 0);
	j = PyTuple_GetItem(arg, 1);
	if (!(self->v.size() <= PyLong_AsLong(i)) && (self->v[0].size() <= PyLong_AsLong(j)))
	{
		PyErr_SetString(PyExc_IndexError, "list index out of range");
		return NULL;
	}
	else
	{
		return Py_BuildValue("f", self->v[PyLong_AsLong(i)][PyLong_AsLong(j)]);
	}
}

static PyObject*
matr_repr(myMatrixObject* self)
{
	if (self->v.size() ==0) {
		return PyUnicode_FromString("<Matr {}>");
	}
	_PyUnicodeWriter writer;
	_PyUnicodeWriter_Init(&writer);
	writer.overallocate = 1;
	writer.min_length = 10;
	PyObject* el_str = NULL;
	PyObject* el;


	if (_PyUnicodeWriter_WriteASCIIString(&writer, "<Matr { ", 8) < 0) {
		goto error;
	}

	for (auto el : self->v) {
		if (_PyUnicodeWriter_WriteASCIIString(&writer, "{", 1) < 0)
		{
			goto error;
		}
		for (double l : el) {
			el_str = PyUnicode_FromFormat("%f", l);
			if (_PyUnicodeWriter_WriteStr(&writer, el_str)) {
				Py_DECREF(el_str);
				goto error;
			}
			Py_DECREF(el_str);

		}
		if (_PyUnicodeWriter_WriteASCIIString(&writer, "}", 1) < 0)
		{
			goto error;
		}
	}

	writer.overallocate = 0;
	if (_PyUnicodeWriter_WriteASCIIString(&writer, "}>", 2) < 0) {
		goto error;
	}

	return _PyUnicodeWriter_Finish(&writer);

error:
	Py_XDECREF(el_str);
	_PyUnicodeWriter_Dealloc(&writer);
	return NULL;


}

static PyObject* matr_str(myMatrixObject* self)
{

	PyObject* el;
	for (auto el : self->v) {
		for (int i = 0; i < el.size(); i++) {
			cout << el[i] << " ";
		}
		cout << endl;
	}
	return 0;
}
static int
matr_contains(myMatrixObject* self, PyObject* arg)
{
	if (!PyLong_Check(arg)) {
		PyErr_SetString(PyExc_TypeError, "object must be number");
		return -1;
	}

	long int num = PyLong_AsLong(arg);



	for (int i = 0; i < self->v.size(); i++)
	{
		if (std::find(self->v[i].begin(), self->v[i].end(), num) != self->v[i].end())
			return 1;
	}
	return 0;
}

static Py_ssize_t
matrix_length(myMatrixObject* self)
{
	return Py_ssize_t(self->v.size());
}
static PyObject* myMatr_transpose(myMatrixObject* self)
{

	if (self->v.empty())
	{
		PyErr_SetString(PyExc_ValueError, "matrix is empty");
		return NULL;
	}
	else
	{
		PyObject* new_matrix = matr_new(&matr_Type, NULL, NULL);
		myMatrixObject* temp = (myMatrixObject*)new_matrix;
		temp->v.resize(self->v[0].size(), vector<float>(self->v.size()));

		for (int i = 0; i < self->v[0].size(); i++)
		{
			for (int j = 0; j < self->v.size(); j++)
			{
				temp->v[i][j] = self->v[j][i];
			}
		}
		return (PyObject*)temp;
	}
	//return (PyObject*)temp;
}

static PyObject* myMatr_add(myMatrixObject* self, PyObject* args)
{
	PyObject* matr_a;
	PyObject* matr_b;
	if ((!PyArg_ParseTuple(args, "OO", &matr_a, &matr_b)))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be a matrix type.");
		return NULL;
	}
	myMatrixObject* matrA = (myMatrixObject*)matr_a;
	myMatrixObject* matrB = (myMatrixObject*)matr_b;
	PyObject* new_matrix = matr_new(&matr_Type, NULL, NULL);
	myMatrixObject* temp = (myMatrixObject*)new_matrix;
	temp->v.resize(matrA->v.size(), vector<float>(matrA->v[0].size()));
	for (int i = 0; i < matrA->v.size(); i++)
	{
		for (int j = 0; j < matrA->v[i].size(); j++)
		{
			temp->v[i][j] = matrA->v[i][j] + matrB->v[i][j];
		}
	}
	return (PyObject*)temp;
}

static PyObject* myMatr_mul(myMatrixObject* self, PyObject* args)
{
	PyObject* matr_a;
	PyObject* matr_b;
	if ((!PyArg_ParseTuple(args, "OO", &matr_a, &matr_b)))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be a matrix type.");
		return NULL;
	}

	myMatrixObject* matrA = (myMatrixObject*)matr_a;
	myMatrixObject* matrB = (myMatrixObject*)matr_b;
	PyObject* new_matrix = matr_new(&matr_Type, NULL, NULL);
	myMatrixObject* temp = (myMatrixObject*)new_matrix;
	if (matrA->v.size() != matrB->v[0].size())
	{
		PyErr_SetString(PyExc_ArithmeticError, "Number of columns of first matrix must me equal to number of raws of second matrix.");
		return NULL;
	}
	else
	{
		temp->v.resize(matrA->v.size(), vector<float>(matrB->v[0].size()));
		for (int i = 0; i < matrA->v.size(); i++)
		{
			for (int j = 0; j < matrB->v[0].size(); j++)
			{
				temp->v[i][j] = 0;
				for (int k = 0; k < matrA->v[0].size(); k++)
				{
					temp->v[i][j] += matrA->v[i][k] * matrB->v[k][j];
				}
			}
		}
		return (PyObject*)temp;
	}


}

static PyObject* myMatr_add_val(myMatrixObject* m, PyObject* args)
{
	long int l;
	printf("Adds a value");
	if (!PyArg_ParseTuple(args, "l", &l))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be a int.");
		return NULL;
	}
	PyObject* new_matrix = matr_new(&matr_Type, NULL, NULL);
	myMatrixObject* temp = (myMatrixObject*)new_matrix;
	temp->v.resize(m->v.size(), vector<float>(m->v[0].size()));
	for (int i = 0; i < m->v.size(); i++)
	{
		for (int j = 0; j < m->v[i].size(); j++)
		{
			temp->v[i][j] += m->v[i][j]+l;
		}
	}

	return (PyObject*)temp;
}
static PyObject* myMatr_div_val(myMatrixObject* self,PyObject* args)
{
	PyObject* matr;
	long int l;
	printf("Adds a value");
	if (!PyArg_ParseTuple(args, "Ol", &matr,&l))
	{
		PyErr_SetString(PyExc_TypeError, "parameter must be a matrix and int.");
		return NULL;
	}
	myMatrixObject* Matr = (myMatrixObject*)matr;
	PyObject* new_matrix = matr_new(&matr_Type, NULL, NULL);
	myMatrixObject* temp = (myMatrixObject*)new_matrix;
	temp->v.resize(Matr->v.size(), vector<float>(Matr->v[0].size()));
	for (int i = 0; i < Matr->v.size(); i++)
	{
		for (int j = 0; j < Matr->v[i].size(); j++)
		{
			temp->v[i][j] = Matr->v[i][j]/l;
		}
	}

	return (PyObject*)temp;
}



// Our Module Definition struct
static struct PyModuleDef myMatrix = {
	PyModuleDef_HEAD_INIT,
	"myMatrix",
	"Test Module",
	-1,
	matr_methods
};

// Initializes our module using our above struct
PyMODINIT_FUNC PyInit_myMatrix(void)
{
	PyObject* m;
	if (PyType_Ready(&matr_Type) < 0)
		return NULL;

	m = PyModule_Create(&myMatrix);
	if (m == NULL)
		return NULL;

	Py_INCREF(&matr_Type);
	PyModule_AddObject(m, "Matrix", (PyObject*)& matr_Type);
	return m;
}

