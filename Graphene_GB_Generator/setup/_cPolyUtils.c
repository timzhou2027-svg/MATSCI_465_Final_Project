#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* --- 1. 文档字符串 --- */
static char module_docstring[] = "This module provides python interface to various utility functions written in C for the grainBdr class.";
static char vorCentroid_docstring[] = "Input: start, end, regions, point_region, points_x, points_y, vertices_x, vertices_y, out_x, out_y";
static char closest_docstring[] = "Input: p1, p2. For each point in p1, returns the index of the point in p2 that its closest to.";
static char selfClosest_docstring[] = "Input: p1. For each point in p1, returns the index of the point in p1 that its closest to.";

/* --- 2. 函数原型声明 --- */
static PyObject * cPolyUtils_vorCentroid(PyObject *self, PyObject *args);
static PyObject * cPolyUtils_closest(PyObject *self, PyObject *args);
static PyObject * cPolyUtils_selfClosest(PyObject *self, PyObject *args);

/* --- 3. 方法映射表 --- */
static PyMethodDef module_methods[] = {
    {"vorCentroid", cPolyUtils_vorCentroid, METH_VARARGS, vorCentroid_docstring},
    {"closest", cPolyUtils_closest, METH_VARARGS, closest_docstring},
    {"selfClosest", cPolyUtils_selfClosest, METH_VARARGS, selfClosest_docstring},
    {NULL, NULL, 0, NULL}
};

/* --- 4. Python 3 模块定义结构体 --- */
static struct PyModuleDef _cPolyUtilsmodule = {
    PyModuleDef_HEAD_INIT,
    "_cPolyUtils", 
    module_docstring, 
    -1, 
    module_methods
};

/* --- 5. Python 3 模块初始化入口 --- */
PyMODINIT_FUNC PyInit__cPolyUtils(void) {
    PyObject *m = PyModule_Create(&_cPolyUtilsmodule);
    if (m == NULL)
        return NULL;

    /* 加载 Numpy 功能，必须调用否则数组操作会崩溃 */
    import_array();
    
    return m;
}

/* --- 6. 函数具体实现 --- */

static PyObject *cPolyUtils_selfClosest(PyObject *self, PyObject *args)
{
    double lx=0.f, ly=0.f;
    PyObject *p1_x, *p1_y, *ind_out, *dist_out;
    PyArrayObject *p1_x_arr, *p1_y_arr, *ind_out_arr, *dist_out_arr;
    
    if (!PyArg_ParseTuple(args, "ddO!O!O!O!", &lx, &ly, &PyArray_Type, &p1_x, &PyArray_Type, &p1_y, &PyArray_Type, &ind_out, &PyArray_Type, &dist_out)) return NULL;

    p1_x_arr = (PyArrayObject *)PyArray_FROM_OTF(p1_x, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    p1_y_arr = (PyArrayObject *)PyArray_FROM_OTF(p1_y, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    ind_out_arr = (PyArrayObject *)PyArray_FROM_OTF(ind_out, NPY_DOUBLE, NPY_ARRAY_INOUT_ARRAY);
    dist_out_arr = (PyArrayObject *)PyArray_FROM_OTF(dist_out, NPY_DOUBLE, NPY_ARRAY_INOUT_ARRAY);

    double *ct_ind = (double *)PyArray_DATA(ind_out_arr);
    double *ct_dist = (double *)PyArray_DATA(dist_out_arr);
    double *ct_p1x = (double *)PyArray_DATA(p1_x_arr);
    double *ct_p1y = (double *)PyArray_DATA(p1_y_arr);

    int nP1 = PyArray_SIZE(p1_x_arr);

    for(int i = 0; i < nP1 - 1; i++) {
        double minDist = -1.0;
        int minInd = -1;
        for (int j = i + 1; j < nP1; j++) {
            double dx = fabs(ct_p1x[i] - ct_p1x[j]);
            double dy = fabs(ct_p1y[i] - ct_p1y[j]);
            dx = dx > lx/2 ? lx - dx : dx;
            dy = dy > ly/2 ? ly - dy : dy;
            double dist = sqrt(dx*dx + dy*dy);
            
            if(minDist < 0 || dist < minDist) {
                minDist = dist;
                minInd = j;
            }
        }
        ct_ind[i] = (double)minInd;
        ct_dist[i] = minDist;
    }

    Py_DECREF(p1_x_arr); Py_DECREF(p1_y_arr);
    Py_DECREF(ind_out_arr); Py_DECREF(dist_out_arr);
    return Py_BuildValue("i", 0);
}

static PyObject *cPolyUtils_closest(PyObject *self, PyObject *args)
{
    PyObject *p1_x, *p1_y, *p2_x, *p2_y, *ind_out;
    PyArrayObject *p1_x_arr, *p1_y_arr, *p2_x_arr, *p2_y_arr, *ind_out_arr;
    
    if (!PyArg_ParseTuple(args, "O!O!O!O!O!", &PyArray_Type, &p1_x, &PyArray_Type, &p1_y, &PyArray_Type, &p2_x, &PyArray_Type, &p2_y, &PyArray_Type, &ind_out)) return NULL;

    p1_x_arr = (PyArrayObject *)PyArray_FROM_OTF(p1_x, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    p1_y_arr = (PyArrayObject *)PyArray_FROM_OTF(p1_y, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    p2_x_arr = (PyArrayObject *)PyArray_FROM_OTF(p2_x, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    p2_y_arr = (PyArrayObject *)PyArray_FROM_OTF(p2_y, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    ind_out_arr = (PyArrayObject *)PyArray_FROM_OTF(ind_out, NPY_DOUBLE, NPY_ARRAY_INOUT_ARRAY);

    double *ct_ind = (double *)PyArray_DATA(ind_out_arr);
    double *ct_p1x = (double *)PyArray_DATA(p1_x_arr);
    double *ct_p1y = (double *)PyArray_DATA(p1_y_arr);
    double *ct_p2x = (double *)PyArray_DATA(p2_x_arr);
    double *ct_p2y = (double *)PyArray_DATA(p2_y_arr);

    int nP1 = PyArray_SIZE(p1_x_arr);
    int nP2 = PyArray_SIZE(p2_x_arr);

    for(int i = 0; i < nP1; i++) {
        double minDist = -1.0;
        int minInd = -1;
        for (int j = 0; j < nP2; j++) {
            double dx = ct_p1x[i] - ct_p2x[j];
            double dy = ct_p1y[i] - ct_p2y[j];
            double dist2 = dx*dx + dy*dy;
            if(minDist < 0 || dist2 < minDist) {
                minDist = dist2;
                minInd = j;
            }
        }
        ct_ind[i] = (double)minInd;
    }

    Py_DECREF(p1_x_arr); Py_DECREF(p1_y_arr);
    Py_DECREF(p2_x_arr); Py_DECREF(p2_y_arr);
    Py_DECREF(ind_out_arr);
    return Py_BuildValue("i", 0);
}

static PyObject *cPolyUtils_vorCentroid(PyObject *self, PyObject *args)
{
    int start, end;
    PyObject *point_region, *regions, *points_x, *points_y, *verts_x, *verts_y, *out_x, *out_y;
    PyArrayObject *point_region_arr, *verts_x_arr, *verts_y_arr, *out_x_arr, *out_y_arr, *points_x_arr, *points_y_arr;

    if (!PyArg_ParseTuple(args, "iiO!O!O!O!O!O!O!O!", &start, &end, &PyList_Type, &regions, &PyArray_Type, &point_region, &PyArray_Type, &points_x, &PyArray_Type, &points_y, &PyArray_Type, &verts_x, &PyArray_Type, &verts_y, &PyArray_Type, &out_x, &PyArray_Type, &out_y)) return NULL;

    point_region_arr = (PyArrayObject *)PyArray_FROM_OTF(point_region, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    verts_x_arr = (PyArrayObject *)PyArray_FROM_OTF(verts_x, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    verts_y_arr = (PyArrayObject *)PyArray_FROM_OTF(verts_y, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    points_x_arr = (PyArrayObject *)PyArray_FROM_OTF(points_x, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    points_y_arr = (PyArrayObject *)PyArray_FROM_OTF(points_y, NPY_DOUBLE, NPY_ARRAY_IN_ARRAY);
    out_x_arr = (PyArrayObject *)PyArray_FROM_OTF(out_x, NPY_DOUBLE, NPY_ARRAY_INOUT_ARRAY);
    out_y_arr = (PyArrayObject *)PyArray_FROM_OTF(out_y, NPY_DOUBLE, NPY_ARRAY_INOUT_ARRAY);

    double *ct_pt_reg = (double *)PyArray_DATA(point_region_arr);
    double *ct_vx = (double *)PyArray_DATA(verts_x_arr);
    double *ct_vy = (double *)PyArray_DATA(verts_y_arr);
    double *ct_px = (double *)PyArray_DATA(points_x_arr);
    double *ct_py = (double *)PyArray_DATA(points_y_arr);
    double *ct_ox = (double *)PyArray_DATA(out_x_arr);
    double *ct_oy = (double *)PyArray_DATA(out_y_arr);

    for(int pt = start; pt < end; pt++) {
        int regNum = (int)ct_pt_reg[pt];
        PyObject *vert_list = PyList_GetItem(regions, regNum);
        int vert_list_len = PyList_Size(vert_list);
        double *reg_verts_x = (double *)malloc((vert_list_len + 1) * sizeof(double));
        double *reg_verts_y = (double *)malloc((vert_list_len + 1) * sizeof(double));
        int isOpen = 0;

        for(int j = 0; j < vert_list_len; j++) {
            PyObject *vert_num = PyList_GetItem(vert_list, j);
            /* --- 修改点：PyLong_AsLong 适配 Python 3 --- */
            long vert_num_int = PyLong_AsLong(vert_num);
            if (vert_num_int == -1) { isOpen = 1; break; }
            reg_verts_x[j] = ct_vx[vert_num_int];
            reg_verts_y[j] = ct_vy[vert_num_int];
        }

        if(isOpen) {
            ct_ox[pt - start] = ct_px[pt];
            ct_oy[pt - start] = ct_py[pt];
        } else {
            reg_verts_x[vert_list_len] = reg_verts_x[0];
            reg_verts_y[vert_list_len] = reg_verts_y[0];
            double area = 0, cx = 0, cy = 0;
            for(int j = 0; j < vert_list_len; j++) {
                double t = reg_verts_x[j]*reg_verts_y[j+1] - reg_verts_x[j+1]*reg_verts_y[j];
                area += t;
                cx += (reg_verts_x[j] + reg_verts_x[j+1]) * t;
                cy += (reg_verts_y[j] + reg_verts_y[j+1]) * t;
            }
            ct_ox[pt - start] = cx / (3.0 * area);
            ct_oy[pt - start] = cy / (3.0 * area);
        }
        free(reg_verts_x); free(reg_verts_y);
    }

    Py_DECREF(point_region_arr); Py_DECREF(verts_x_arr); Py_DECREF(verts_y_arr);
    Py_DECREF(points_x_arr); Py_DECREF(points_y_arr);
    Py_DECREF(out_x_arr); Py_DECREF(out_y_arr);
    return Py_BuildValue("i", 0);
}