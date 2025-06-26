#include <Python.h>
#include "shm/add.h"

static PyObject* py_add(PyObject*, PyObject* args) {
    int a, b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b))
        return nullptr;
    int result = add(a, b);
    return PyLong_FromLong(result);
}

static PyMethodDef ShmMethods[] = {
    {"add", py_add, METH_VARARGS, "Add two integers"},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef shmModule = {
    PyModuleDef_HEAD_INIT,
    "shm_py",
    "Python bindings for shm",
    -1,
    ShmMethods
};

PyMODINIT_FUNC PyInit_shm_py(void) {
    return PyModule_Create(&shmModule);
}
