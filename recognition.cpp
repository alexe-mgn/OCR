#include <Python.h>

#include <exception>
#include <unistd.h>

#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/QRect>
#include <QtCore/QRegExp>

#include "recognition.h"


QString pyTracebackMessage() {
    QString msg;
    PyObject *ptype, *pvalue, *ptraceback;
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    if (ptype != nullptr) {
        // Check format available
        PyObject *tb = PyImport_ImportModule("traceback");
        PyObject *tb_format = nullptr;
        if (tb != nullptr)
            tb_format = PyObject_GetAttrString(tb, "format_exception");
        Py_XDECREF(tb);

        if (tb_format != nullptr) {
            // Fill args
            PyObject *args = PyTuple_New(3);
            {
                int n = 0;
                PyObject *perr = PyObject_CallOneArg(ptype, pvalue != nullptr ? pvalue : Py_None);
                for (PyObject *i : {ptype, perr, ptraceback}) {
                    Py_XINCREF(i);
                    PyTuple_SetItem(args, n++, i != nullptr ? i : Py_None);
                }
                Py_DECREF(perr);
            }
            // Call
            PyObject *lines = PyObject_Call(tb_format, args, nullptr);
            Py_DECREF(args);
            int n = PyObject_Length(lines);
            // Message
            for (int i = 0; i < n; ++i) {
                PyObject *line = PyList_GetItem(lines, 0);
                msg.append(PyUnicode_AsUTF8(line));
                PySequence_DelItem(lines, 0);
            }
            Py_XDECREF(lines);
        } else {
            for (PyObject *i : {ptype, pvalue, ptraceback}) {
                if (i != nullptr) {
                    PyObject *i_str = PyObject_Str(i);
                    if (i_str != nullptr) {
                        msg.append(PyUnicode_AsUTF8(i_str));
                        Py_DECREF(i_str);
                        msg.append('\n');
                    }
                }
            }
        }
        Py_XDECREF(tb_format);

        PyErr_Restore(ptype, pvalue, ptraceback);
        Py_XDECREF(ptype);
        Py_XDECREF(pvalue);
        Py_XDECREF(ptraceback);
    }
    return msg;
}

void *ImageCR::rec_f = nullptr;

QString ImageCR::getPythonHome() {
    QString path = "python";
    QFile ph_file("PYTHONHOME.txt");
    if (ph_file.open(QFile::ReadOnly)) {
        path = ph_file.readLine();
        path.remove(QRegExp("[\\n\\r]{1,2}$"));
    }
    ph_file.close();
    return path;
}

void ImageCR::init() {
    if (rec_f == nullptr || !Py_IsInitialized()) {
        if (!Py_IsInitialized()) {
            Py_SetPythonHome(ImageCR::getPythonHome().toStdWString().c_str());
            Py_Initialize();
        }

        // REFERENCE LEAK
        PyImport_ImportModule("traceback");

//        PyRun_SimpleString("import sys\nsys.path.append('../..')\n");
//        PyRun_SimpleString("import os\nimport sys\nprint(os.getcwd())\nprint(sys.path)\n");
        PyObject *rec = PyImport_ImportModule("recognition");
        if (rec == nullptr || PyErr_Occurred()) {
            QString msg = pyTracebackMessage();
            msg.insert(0, "Could not import python recognition module.\n");
            throw std::runtime_error(msg.toStdString());
        }
        rec_f = PyObject_GetAttrString(rec, "bytes_predict_letters");
        if (rec_f == nullptr || PyErr_Occurred()) {
            QString msg = pyTracebackMessage();
            msg.insert(0, "Could not import recognition function from python module.\n");
            throw std::runtime_error(msg.toStdString());
        }

        Py_DECREF(rec);
    }
}

void ImageCR::finalize() {
    if (rec_f != nullptr)
        Py_DECREF(rec_f);
    Py_Finalize();
}

QList<TextItem *> ImageCR::scan(const QImage &image) {
    init();
    // Get image bytes
    QByteArray q_img_bytes;
    QBuffer buff(&q_img_bytes);
    buff.open(QBuffer::WriteOnly);
    image.save(&buff, "JPG");
    buff.close();
    // Transfer to python
    PyObject *p_img_bytes = PyBytes_FromStringAndSize(q_img_bytes.constData(), q_img_bytes.size());
    if (p_img_bytes == nullptr || PyErr_Occurred()) {
        QString msg = pyTracebackMessage();
        msg.insert(0, "Python image conversion error.\n");
        throw std::runtime_error(msg.toStdString());
    }
    PyObject *items = PyObject_CallOneArg(static_cast<PyObject *>(rec_f), p_img_bytes);
    if (items == nullptr || PyErr_Occurred()) {
        QString msg = pyTracebackMessage();
        msg.insert(0, "Python recognition error.\n");
        throw std::runtime_error(msg.toStdString());
    }
    Py_DECREF(p_img_bytes);
    int n = PyObject_Length(items);

    QList<TextItem *> letters;
    for (int i = 0; i < n; ++i) {
        PyObject *item = PyList_GetItem(items, i);
        PyObject *ix = PyTuple_GetItem(item, 0),
                *iy = PyTuple_GetItem(item, 1),
                *iw = PyTuple_GetItem(item, 2),
                *ih = PyTuple_GetItem(item, 3),
                *it = PyTuple_GetItem(item, 4);
        letters.append(new TextItem(
                QRect(
                        PyLong_AsLong(ix),
                        PyLong_AsLong(iy),
                        PyLong_AsLong(iw),
                        PyLong_AsLong(ih)),
                PyUnicode_AsUTF8(it)));
    }
    Py_DECREF(items);
//    return letters;
    QList<TextItem *> words = joinLetters(letters);
    while (!letters.empty())
        delete letters.takeFirst();
    return words;
}
