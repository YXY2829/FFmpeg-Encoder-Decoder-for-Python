#ifndef MPEGPYD_H_INCLUDED
#define MPEGPYD_H_INCLUDED

#include <iostream>
#include <Python.h>
#include <numpy/arrayobject.h>
#include <structmember.h>
#include <string>
#include <sstream>
#include "MpegCoder.h"
#include "MpegStreamer.h"
using std::string;
using std::ostringstream;

PyObject *str2PyStr(string Str) {
    //��ԭʼ���תΪUnicode
    int wlen = mbstowcs(nullptr, Str.c_str(), 0);
    wchar_t* wszString = new wchar_t[wlen + 1];
    mbstowcs(wszString, Str.c_str(), wlen);
    wszString[wlen] = 0;
    PyObject* res = PyUnicode_FromUnicode((const Py_UNICODE*)wszString, wlen);
    delete[] wszString;
    return res;
}

/*****************************************************************************
* ��/�ṹ�Ķ���:
* ֱ������CMISS_Handle����
* ����֮�⣬���ڸ����������κ�Python�Կڵ����ݣ�
* ��Ϊ��Ӧ�������Ѿ���װ��CMISS_Handle��
*****************************************************************************/
typedef struct _C_MpegDecoder
{
    PyObject_HEAD             // == PyObject ob_base;  ����һ��PyObject����.
                              //////////////////////////////////////////////////////////////////////////
                              // ��/�ṹ��������Ա����.
                              //
    cmpc::CMpegDecoder *_in_Handle;
}C_MpegDecoder;

typedef struct _C_MpegEncoder
{
    PyObject_HEAD             // == PyObject ob_base;  ����һ��PyObject����.
                              //////////////////////////////////////////////////////////////////////////
                              // ��/�ṹ��������Ա����.
                              //
    cmpc::CMpegEncoder *_in_Handle;
}C_MpegEncoder;

typedef struct _C_MpegClient
{
    PyObject_HEAD             // == PyObject ob_base;  ����һ��PyObject����.
                              //////////////////////////////////////////////////////////////////////////
                              // ��/�ṹ��������Ա����.
                              //
        cmpc::CMpegClient *_in_Handle;
}C_MpegClient;

static PyMemberDef C_MPDC_DataMembers[] =        //ע����/�ṹ�����ݳ�Ա.
{ //��ע���κ����ݣ���Ϊ������CMpegDecoder���ϲ��ǲ��ɼ���
  //{"m_dEnglish", T_FLOAT,  offsetof(CScore, m_dEnglish), 0, "The English score of instance."},
    { "hAddress",   T_ULONGLONG, offsetof(C_MpegDecoder, _in_Handle),   READONLY, "The address of the handle in memory." },
    { nullptr, 0, 0, 0, nullptr }
};

static PyMemberDef C_MPEC_DataMembers[] =        //ע����/�ṹ�����ݳ�Ա.
{ //��ע���κ����ݣ���Ϊ������CMpegDecoder���ϲ��ǲ��ɼ���
  //{"m_dEnglish", T_FLOAT,  offsetof(CScore, m_dEnglish), 0, "The English score of instance."},
    { "hAddress",   T_ULONGLONG, offsetof(C_MpegEncoder, _in_Handle),   READONLY, "The address of the handle in memory." },
    { nullptr, 0, 0, 0, nullptr }
};

static PyMemberDef C_MPCT_DataMembers[] =        //ע����/�ṹ�����ݳ�Ա.
{ //��ע���κ����ݣ���Ϊ������CMpegDecoder���ϲ��ǲ��ɼ���
  //{"m_dEnglish", T_FLOAT,  offsetof(CScore, m_dEnglish), 0, "The English score of instance."},
    { "hAddress",   T_ULONGLONG, offsetof(C_MpegClient, _in_Handle),   READONLY, "The address of the handle in memory." },
    { nullptr, 0, 0, 0, nullptr }
};

/*****************************************************************************
* ȫ��������:
* Ϊ�����ĺ���ע��׼���������Ķ���
*****************************************************************************/
/*static void C_CMISS_init(C_CMISS_h* Self, PyObject* pArgs);
static void C_CMISS_Destruct(C_CMISS_h* Self);
static PyObject* C_CMISS_Str(C_CMISS_h* Self);
static PyObject* C_CMISS_Repr(C_CMISS_h* Self);
static PyObject* C_CMISS_login(C_CMISS_h* Self, PyObject *args, PyObject *kwargs);
static PyObject* C_CMISS_set_config(C_CMISS_h* Self, PyObject *args, PyObject *kwargs);
static PyObject* C_CMISS_save_config(C_CMISS_h* Self, PyObject* args);
static PyObject* C_CMISS_load_config(C_CMISS_h* Self, PyObject* args);
static PyObject* C_CMISS_save_as_file(C_CMISS_h* Self);
static PyObject* C_CMISS_download_bin_file(C_CMISS_h* Self);
static PyObject* C_CMISS_save_as_file_XML(C_CMISS_h* Self, PyObject* args);
PyMODINIT_FUNC PyInit_Score(void);*/

static PyObject* C_MPC_Global(PyObject* Self, PyObject *args, PyObject *kwargs) {
    char dumpLevel = -1;
    static char *kwlist[] = { "dumpLevel", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|B", kwlist, &dumpLevel)) {
        PyErr_SetString(PyExc_TypeError, "Error.GlobalSettings: invalid keyword'");
        return nullptr;
    }
    if (dumpLevel != -1) {
        cmpc::__dumpControl = static_cast<int8_t>(dumpLevel);
    }
    Py_RETURN_NONE;
}

static PyObject* C_MPC_Help(PyObject* Self) {
    cout << R"(================================================================================
      __   _                         _ _ _                ,___            
     ( /  /        /        o       ( / ) )              /   /     /      
      (__/ , , _, /_  _  _ _'  (     / / /  ,_   _  _,  /    __ __/ _  _  
       _/_(_/_(__/ /_(/_/ / /_/_)_  / / (__/|_)_(/_(_)_(___/(_)(_/_(/_/ (_
      //                                   /|       /|                    
     (/                                   (/       (/                     
================================================================================
Yuchen's Mpeg Coder - Readme
    This is a mpegcoder adapted from FFmpeg & Python-c-api.Using it you could 
        get access to processing video easily. Just use it as a common module in 
        python like this.
        >>> import mpegCoder
    Noted that this API need you to install numpy.
    An example of decoding a video in an arbitrary format:
        >>> d = mpegCoder.MpegDecoder()
        >>> d.FFmpegSetup(b'inputVideo.mp4')
        >>> p = d.ExtractGOP(10) # Get a gop of current video by setting the 
                                   start position of 10th frame.
        >>> p = d.ExtractGOP() # Get a gop of current video, using the current 
                                 position after the last ExtractGOP.
        >>> d.ExtractFrame(100, 100) # Extract 100 frames from the begining of 
                                       100th frame.
    An example of transfer the coding of a video with an assigned codec:
        >>> d = mpegCoder.MpegDecoder()
        >>> d.FFmpegSetup(b'i.avi')
        >>> e = mpegCoder.MpegEncoder()
        >>> e.setParameter(decoder=d, codecName=b'libx264', videoPath=b'o.mp4')
            # inherit most of parameters from the decoder.
        >>> opened = e.FFmpegSetup() # Load the encoder.
        >>> if opened: # If encoder is not loaded successfully, do not continue.
        ...     p = True
        ...     while p:
        ...         p = d.ExtractGOP() # Extract current GOP.
        ...         for i in p: # Select every frame.
        ...             e.EncodeFrame(i) # Encode current frame.
        ...     e.FFmpegClose() # End encoding, and flush all frames in cache.
        >>> d.clear() # Close the input video.
    An example of demuxing the video streamer from a server:
        >>> d = mpegCoder.MpegClient() # create the handle
        >>> d.setParameter(dstFrameRate=(5,1), readSize=5, cacheSize=12)
            # normalize the frame rate to 5 FPS, and use a cache which size is 
            # 12 frames. Read 5 frames each time.
        >>> success = d.FFmpegSetup(b'rtsp://localhost:8554/video')
        >>> if not success: # exit if fail to connect with the server
        ...     exit()
        >>> d.start() # start the sub-thread for demuxing the stream.
        >>> for i in range(10): # processing loop
        ...     time.sleep(5)
        ...     p = d.ExtractFrame() # every 5 seconds, read 5 frames (1 sec.)
        ...     # do some processing
        >>> d.terminate() # shut down the current thread. You could call start() 
                          # and let it restart.
        >>> d.clear() # Disconnect with the stream.
    For more instructions, you could tap help(mpegCoder). 
================================================================================
V2.0 update report:
    1. Revise the bug of the encoder which may cause the stream duration is shorter
       than the real duration of the video in some not advanced media players.
    2. Improve the structure of the code and remove some unnecessary codes.
    3. Provide a complete version of client, which could demux the video stream
       from a server in any network protocol.
V1.8 update report:
    1. Provide options (widthDst, heightDst) to let MpegDecoder could control the
       output size manually. To ensure the option is valid, we must use the method
        'setParameter' before 'FFmpegSetup'.
    2. Optimize some realization of Decoder so that its efficiency could be 
       improved.
V1.7 update report:
    1. Realize the encoder totally.
    2. Provide a global option 'dumpLevel' to control the log shown in the screen.
    3. Fix bugs in initalize functions.
V1.5 update report:
    1. Provide an incomplete version of encoder, which could encode frames as a 
       video stream that could not be played by player.
V1.4 update report:
    1. Fix a severe bug of the decoder, which causes the memory collapsed if 
       decoding a lot of frames.
V1.2 update report:
    1. Use numpy array to replace the native pyList, which improves the speed 
       significantlly.
V1.0 update report:
    1. Provide the decoder which could decode videos in arbitrary formats and 
       arbitrary coding.
)";
    Py_RETURN_NONE;
}

/*****************************************************************************
* ����C_CMISS_h����������á����췽����
*****************************************************************************/
static int C_MPDC_init(C_MpegDecoder* Self, PyObject* args, PyObject *kwargs) { //���ء����췽��.
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoPath", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.Initialize: need 'videoPath(string)'" );
        return -1;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    Self->_in_Handle = new cmpc::CMpegDecoder;
    if (!in_vpath.empty())
        Self->_in_Handle->FFmpegSetup(in_vpath);

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    //cout << sizeof(Self->_in_Handle) << " - " << sizeof(unsigned long long) << endl;
    return 0;
}

static int C_MPEC_init(C_MpegEncoder* Self) { //���ء����췽��.
    Self->_in_Handle = new cmpc::CMpegEncoder;
    /*if (cmpc::__dumpControl > 1)
        cout << "Create Handle of Encoder." << endl;*/
    return 0;
}

static int C_MPCT_init(C_MpegClient* Self) { //���ء����췽��.
    Self->_in_Handle = new cmpc::CMpegClient;
    /*if (cmpc::__dumpControl > 1)
    cout << "Create Handle of Encoder." << endl;*/
    return 0;
}

static void C_MPDC_Destruct(C_MpegDecoder* Self) { //���ء���������
    delete Self->_in_Handle; //ͨ����̬����/�ͷ�������������Ⱦ�̬����������

                             //�������PyObject*��Ա�Ļ���Ҫһ���ͷ�֮.
                             //�磺Py_XDECREF(Self->Member);
    Py_TYPE(Self)->tp_free((PyObject*)Self); //�ͷŶ���/ʵ��.
}

static void C_MPEC_Destruct(C_MpegEncoder* Self) { //���ء���������
    delete Self->_in_Handle; //ͨ����̬����/�ͷ�������������Ⱦ�̬����������

                             //�������PyObject*��Ա�Ļ���Ҫһ���ͷ�֮.
                             //�磺Py_XDECREF(Self->Member);
    Py_TYPE(Self)->tp_free((PyObject*)Self); //�ͷŶ���/ʵ��.
}

static void C_MPCT_Destruct(C_MpegClient* Self) { //���ء���������
    delete Self->_in_Handle; //ͨ����̬����/�ͷ�������������Ⱦ�̬����������

                             //�������PyObject*��Ա�Ļ���Ҫһ���ͷ�֮.
                             //�磺Py_XDECREF(Self->Member);
    Py_TYPE(Self)->tp_free((PyObject*)Self); //�ͷŶ���/ʵ��.
}

static PyObject* C_MPDC_Str(C_MpegDecoder* Self) { //���ء�����str/printʱ�Զ����ô˺���.
    ostringstream OStr;
    OStr << *(Self->_in_Handle);
    string Str = OStr.str();

    //��ԭʼ���תΪUnicode
    return str2PyStr(Str);
}

static PyObject* C_MPEC_Str(C_MpegEncoder* Self) { //���ء�����str/printʱ�Զ����ô˺���.
    ostringstream OStr;
    OStr << *(Self->_in_Handle);
    string Str = OStr.str();

    //��ԭʼ���תΪUnicode
    return str2PyStr(Str);
}

static PyObject* C_MPCT_Str(C_MpegClient* Self) { //���ء�����str/printʱ�Զ����ô˺���.
    ostringstream OStr;
    OStr << *(Self->_in_Handle);
    string Str = OStr.str();

    //��ԭʼ���תΪUnicode
    return str2PyStr(Str);
}

static PyObject* C_MPDC_Repr(C_MpegDecoder* Self) { //���ء�����repr���ú���ʱ�Զ�����.
    return C_MPDC_Str(Self);
}

static PyObject* C_MPEC_Repr(C_MpegEncoder* Self) { //���ء�����repr���ú���ʱ�Զ�����.
    return C_MPEC_Str(Self);
}

static PyObject* C_MPCT_Repr(C_MpegClient* Self) { //���ء�����repr���ú���ʱ�Զ�����.
    return C_MPCT_Str(Self);
}

/*****************************************************************************
* ����C_MpegDecoder�������Python�ӿڡ�
* C_MPDC_Setup:             ��·��ȡ����Ƶ�������н���������
* C_MPDC_ExtractFrame       ��ȡָ��λ�õ�ָ����Ŀ������֡
*****************************************************************************/
static PyObject* C_MPDC_Setup(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPDC_Setup��������������Ϊ:
    *   videoPath [byte->buffer]: ����ȡ��Ƶ·��
    */
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoPath", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'videoPath(string)'");
        return nullptr;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    bool res;
    if (!in_vpath.empty())
        res = Self->_in_Handle->FFmpegSetup(in_vpath);
    else
        res = Self->_in_Handle->FFmpegSetup();

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    if (res)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* C_MPEC_Setup(C_MpegEncoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPEC_Setup��������������Ϊ:
    *   videoPath [byte->buffer]: ����ȡ��Ƶ·��
    */
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoPath", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'videoPath(string)'");
        return nullptr;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    bool res;
    if (!in_vpath.empty())
        res = Self->_in_Handle->FFmpegSetup(in_vpath);
    else
        res = Self->_in_Handle->FFmpegSetup();

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    if (res)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* C_MPCT_Setup(C_MpegClient* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPCT_Setup��������������Ϊ:
    *   videoAddress [byte->buffer]: ����ȡ��Ƶ·��
    */
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoAddress", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'videoAddress(string)'");
        return nullptr;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    bool res;
    if (!in_vpath.empty())
        res = Self->_in_Handle->FFmpegSetup(in_vpath);
    else
        res = Self->_in_Handle->FFmpegSetup();

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    if (res)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* C_MPDC_resetPath(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPDC_resetPath��������������Ϊ:
    *   videoPath [byte->buffer]: ����ȡ��Ƶ·��
    */
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoPath", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'videoPath(string)'");
        return nullptr;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    Self->_in_Handle->resetPath(in_vpath);

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_resetPath(C_MpegClient* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPCT_resetPath��������������Ϊ:
    *   videoAddress [byte->buffer]: ����ȡ��Ƶ·��
    */
    Py_buffer vpath = { 0 };
    static char *kwlist[] = { "videoAddress", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &vpath)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'videoAddress(string)'");
        return nullptr;
    }
    string in_vpath;
    if (vpath.buf)
        in_vpath.assign(reinterpret_cast<char *>(vpath.buf));
    else
        in_vpath.clear();
    Self->_in_Handle->resetPath(in_vpath);

    in_vpath.clear();
    PyBuffer_Release(&vpath);
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_Start(C_MpegClient* Self) {
    /* ��װ(void)Start���������������� */
    auto success = Self->_in_Handle->start();
    if (!success) {
        PyErr_SetString(PyExc_ConnectionError, "Error.Start: before call this method, need to call FFmpegSetup() successfully, and also you should not call it when the decoding thread is running.'");
        return nullptr;
    }
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_Terminate(C_MpegClient* Self) {
    /* ��װ(void)Terminate���������������� */
    Self->_in_Handle->terminate();
    Py_RETURN_NONE;
}

//ע����������������Ϊʲô���ǲ���ҪPy_IN/DECREF�أ���Ϊδ������ʱ������Ҳû��
//ʹ������None�������ֳɷ��ض���
static PyObject* FreePyArray(PyArrayObject *PyArray) {
    if (PyArray_API == nullptr) {
        import_array();
    }
    uint8_t * out_dataptr = (uint8_t *)PyArray_DATA(PyArray);
    delete [] out_dataptr;
    return nullptr;
}
void FreePyList(PyObject *PyList) {
    Py_ssize_t getlen = PyList_Size(PyList);
    for (Py_ssize_t i = 0; i < getlen; i++) {
        PyObject *Item = PyList_GetItem(PyList, i);
        FreePyArray((PyArrayObject*)Item);
    }
    Py_DECREF(PyList);
    PyList_ClearFreeList();
}

static PyObject* C_MPDC_ExtractFrame(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(int)ExtractFrame��������������Ϊ:
    *   framePos [int->int64_t]: �������֡λ��
    */
    if (PyArray_API == nullptr) {
        import_array();
    }
    int64_t framePos = 0, frameNum = 1;
    static char *kwlist[] = { "framePos", "frameNum", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|LL", kwlist, &framePos, &frameNum)) {
        PyErr_SetString(PyExc_TypeError, "Error.ExtractFrame: need 'framePos(int)/frameNum(int)'");
        return nullptr;
    }
    PyObject *PyFrameList = PyList_New(static_cast<Py_ssize_t>(0));
    //cout << framePos << " - " << frameNum << endl;
    bool res = Self->_in_Handle->ExtractFrame(PyFrameList, framePos, frameNum, 0, 0);
    Py_ssize_t getlen = PyList_Size(PyFrameList);
    res = res && (getlen > 0);
    if (res) {
        PyObject *PyFrameArray = PyArray_FromObject(PyFrameList, NPY_UINT8, 4, 4);
        FreePyList(PyFrameList);
        return PyFrameArray;
    }
    else {
        Py_DECREF(PyFrameList);
        PyList_ClearFreeList();
        Py_RETURN_NONE;
    }
}

static PyObject* C_MPDC_ExtractFrame_Time(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(int)ExtractFrame��������������Ϊ:
    *   timePos [float->double]: �������֡λ��
    */
    if (PyArray_API == nullptr) {
        import_array();
    }
    double timePos = 0;
    int64_t frameNum = 1;
    static char *kwlist[] = { "timePos", "frameNum", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|dL", kwlist, &timePos, &frameNum)) {
        PyErr_SetString(PyExc_TypeError, "Error.ExtractFrame_Time: need 'timePos(float)/frameNum(int)'");
        return nullptr;
    }
    PyObject *PyFrameList = PyList_New(static_cast<Py_ssize_t>(0));
    //cout << framePos << " - " << frameNum << endl;
    bool res = Self->_in_Handle->ExtractFrame(PyFrameList, 0, frameNum, timePos, 1);
    Py_ssize_t getlen = PyList_Size(PyFrameList);
    res = res && (getlen > 0);
    if (res) {
        PyObject *PyFrameArray = PyArray_FromObject(PyFrameList, NPY_UINT8, 4, 4);
        FreePyList(PyFrameList);
        return PyFrameArray;
    }
    else {
        Py_DECREF(PyFrameList);
        PyList_ClearFreeList();
        Py_RETURN_NONE;
    }
}

static PyObject* C_MPEC_EncodeFrame(C_MpegEncoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)EncodeFrame��������������Ϊ:
    *   PyArrayFrame [ndarray->PyArrayObject]: �������֡
    */
    if (PyArray_API == nullptr) {
        import_array();
    }
    PyObject *PyArrayFrame = nullptr;
    static char *kwlist[] = { "PyArrayFrame", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|O", kwlist, &PyArrayFrame)) {
        PyErr_SetString(PyExc_TypeError, "Error.EncodeFrame: need 'PyArrayFrame(ndarray)'");
        return nullptr;
    }
    int res = Self->_in_Handle->EncodeFrame(reinterpret_cast<PyArrayObject *>(PyArrayFrame));
    if (res>=0)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* C_MPCT_ExtractFrame(C_MpegClient* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(int)ExtractFrame��������������Ϊ:
    *   framePos [int->int64_t]: �������֡λ��
    */
    int64_t readSize = 0;
    static char *kwlist[] = { "readSize", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|L", kwlist, &readSize)) {
        PyErr_SetString(PyExc_TypeError, "Error.ExtractFrame: need 'readSize(int)'");
        return nullptr;
    }
    PyObject *res = nullptr;
    if (readSize>0)
        res = Self->_in_Handle->ExtractFrame(readSize);
    else
        res = Self->_in_Handle->ExtractFrame();
    if (res) {
        return res;
    }
    else {
        Py_RETURN_NONE;
    }
}

static PyObject* C_MPDC_ExtractGOP(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(int)ExtractGOP��������������Ϊ:
    *   framePos [int->int64_t]: �������֡λ��
    */
    if(PyArray_API == nullptr) {
        import_array();
    }
    int64_t framePos = -1;
    static char *kwlist[] = { "framePos", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|L", kwlist, &framePos)) {
        PyErr_SetString(PyExc_TypeError, "Error.ExtractGOP: need 'framePos(int)'");
        return nullptr;
    }
    PyObject *PyFrameList = PyList_New(static_cast<Py_ssize_t>(0));
    //cout << framePos << " - " << frameNum << endl;
    if (!(framePos < 0))
        Self->_in_Handle->setGOPPosition(framePos);
    bool res = Self->_in_Handle->ExtractGOP(PyFrameList);
    Py_ssize_t getlen = PyList_Size(PyFrameList);
    res = res && (getlen > 0);
    if (res) {
        PyObject *PyFrameArray = PyArray_FromObject(PyFrameList, NPY_UINT8, 4, 4);
        FreePyList(PyFrameList);
        return PyFrameArray;
    }
    else {
        Py_DECREF(PyFrameList);
        PyList_ClearFreeList();
        Py_RETURN_NONE;
    }
}

static PyObject* C_MPDC_ExtractGOP_Time(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(int)ExtractGOP_Time��������������Ϊ:
    *   timePos [float->double]: �������֡λ��
    */
    if (PyArray_API == nullptr) {
        import_array();
    }
    double timePos = -1;
    static char *kwlist[] = { "timePos", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|d", kwlist, &timePos)) {
        PyErr_SetString(PyExc_TypeError, "Error.ExtractGOP_Time: need 'timePos(float)'");
        return nullptr;
    }
    PyObject *PyFrameList = PyList_New(static_cast<Py_ssize_t>(0));
    //cout << framePos << " - " << frameNum << endl;
    if (!(timePos < 0))
        Self->_in_Handle->setGOPPosition(timePos);
    bool res = Self->_in_Handle->ExtractGOP(PyFrameList);
    Py_ssize_t getlen = PyList_Size(PyFrameList);
    res = res && (getlen > 0);
    if (res) {
        PyObject *PyFrameArray = PyArray_FromObject(PyFrameList, NPY_UINT8, 4, 4);
        FreePyList(PyFrameList);
        return PyFrameArray;
    }
    else {
        Py_DECREF(PyFrameList);
        PyList_ClearFreeList();
        Py_RETURN_NONE;
    }
}

static PyObject* C_MPDC_setGOPPosition(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(void)setGOPPosition��������������Ϊ:
    *   framePos [int->int64_t]: �������֡λ��
    *   timePos [float->double]: �������֡λ��
    */
    int64_t framePos = -1;
    double timePos = -1;
    static char *kwlist[] = { "framePos", "timePos", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|Ld", kwlist, &framePos, &timePos)) {
        PyErr_SetString(PyExc_TypeError, "Error.setGOPPosition: need 'framePos(int)'/'timePos(float)'");
        return nullptr;
    }
    if (!(framePos < 0))
        Self->_in_Handle->setGOPPosition(framePos);
    else if (!(timePos < 0))
        Self->_in_Handle->setGOPPosition(timePos);
    Py_RETURN_NONE;
}

static PyObject* C_MPDC_getParam(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPDC_getParam��������������Ϊ:
    *   parameter [byte->buffer]: ��Ҫ��ȡ�Ĳ���
    *       videoPath:    [byte]  ��ǰ��Ƶ·��
    *       width/height: [int]   ֡��/��
    *       frameCount:   [int]   ���ν����֡������
    *       coderName:    [byte]  ����������
    *       duration:     [float] ��Ƶ����(s)
    *       estFrameNum:  [int]   Ԥ������֡��
    *       avgFrameRate  [float] ƽ��֡��
    */
    Py_buffer param = { 0 };
    static char *kwlist[] = { "paramName", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &param)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'paramName(string)'" );
        return nullptr;
    }
    string in_param;
    if (param.buf)
        in_param.assign(reinterpret_cast<char *>(param.buf));
    else
        in_param.clear();
    PyObject * res = Self->_in_Handle->getParameter(in_param);
    in_param.clear();
    PyBuffer_Release(&param);
    return res;
}

static PyObject* C_MPCT_getParam(C_MpegClient* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPCT_getParam��������������Ϊ:
    *   parameter [byte->buffer]: ��Ҫ��ȡ�Ĳ���
    *       videoPath:    [byte]  ��ǰ��Ƶ·��
    *       width/height: [int]   ֡��/��
    *       frameCount:   [int]   ���ν����֡������
    *       coderName:    [byte]  ����������
    *       duration:     [float] ��Ƶ����(s)
    *       estFrameNum:  [int]   Ԥ������֡��
    *       avgFrameRate  [float] ƽ��֡��
    */
    Py_buffer param = { 0 };
    static char *kwlist[] = { "paramName", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|y*", kwlist, &param)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'paramName(string)'");
        return nullptr;
    }
    string in_param;
    if (param.buf)
        in_param.assign(reinterpret_cast<char *>(param.buf));
    else
        in_param.clear();
    PyObject * res = Self->_in_Handle->getParameter(in_param);
    in_param.clear();
    PyBuffer_Release(&param);
    return res;
}

static PyObject* C_MPDC_setParam(C_MpegDecoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(void)C_MPDC_setParam��������������Ϊ:
    *   parameter [byte->buffer]: ��Ҫ��ȡ�Ĳ���
    *       widthDst/heightDst: [int]   Ŀ��֡��/��
    */
    int widthDst = 0;
    int heightDst = 0;
    static char *kwlist[] = { "widthDst", "heightDst", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|ii", kwlist, &widthDst, &heightDst)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'params'" );
        return nullptr;
    }
    if (widthDst>0) {
        Self->_in_Handle->setParameter("widthDst", &widthDst);
    }
    if (heightDst>0) {
        Self->_in_Handle->setParameter("heightDst", &heightDst);
    }
    Py_RETURN_NONE;
}

static PyObject* C_MPEC_setParam(C_MpegEncoder* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(bool)C_MPEC_setParam��������������Ϊ:
    *   parameter [byte->buffer]: ��Ҫ��ȡ�Ĳ���
    *       decode:    [Decoder]  ��������PyObject
    *       videoPath:    [byte]  ��ǰ��Ƶ·��
    *       codecName:    [byte]  ����������
    *       bitRate:      [int]   ����
    *       width/height: [int]   ֡��/��
    *       GOPSize:      [int]   GOP��С
    *       timeBase:     [tuple] ʱ��
    *       frameRate:    [tuple] ֡��
    */
    PyObject *decoder = nullptr;
    Py_buffer videoPath = { 0 };
    Py_buffer codecName = { 0 };
    double bitRate = -1;
    int width = 0;
    int height = 0;
    int widthSrc = 0;
    int heightSrc = 0;
    int GOPSize = 0;
    int MaxBframe = -1;
    PyObject *frameRate = nullptr;
    static char *kwlist[] = { "decoder", "videoPath", "codecName", "bitRate", "width", "height", "widthSrc", "heightSrc", "GOPSize", "maxBframe", "frameRate", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|Oy*y*diiiiiiO", kwlist, &decoder, &videoPath, &codecName, &bitRate, &width, &height, &widthSrc, &heightSrc, &GOPSize, &MaxBframe, &frameRate)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'params'");
        return nullptr;
    }
    string temp_str;
    if (decoder) {
        temp_str.assign(decoder->ob_type->tp_name);
        if (temp_str.compare("mpegCoder.MpegDecoder") == 0) {
            auto decoderPtr = reinterpret_cast<C_MpegEncoder *>(decoder);
            Self->_in_Handle->setParameter("decoder", decoderPtr->_in_Handle);
        }
        else {
            cerr << "Warning.setParameter: Not intended decoder type, no valid update in this step." << endl;
        }
    }
    if (videoPath.buf) {
        temp_str.assign(reinterpret_cast<char *>(videoPath.buf));
        Self->_in_Handle->setParameter("videoPath", &temp_str);
    }
    if (codecName.buf) {
        temp_str.assign(reinterpret_cast<char *>(codecName.buf));
        Self->_in_Handle->setParameter("codecName", &temp_str);
    }
    if (bitRate>0) {
        auto p = static_cast<int64_t>(bitRate * 1024);
        Self->_in_Handle->setParameter("bitRate", &p);
    }
    if (width>0) {
        Self->_in_Handle->setParameter("width", &width);
    }
    if (height>0) {
        Self->_in_Handle->setParameter("height", &height);
    }
    if (widthSrc>0) {
        Self->_in_Handle->setParameter("widthSrc", &widthSrc);
    }
    if (heightSrc>0) {
        Self->_in_Handle->setParameter("heightSrc", &heightSrc);
    }
    if (GOPSize>0) {
        Self->_in_Handle->setParameter("GOPSize", &GOPSize);
    }
    if (MaxBframe>=0) {
        Self->_in_Handle->setParameter("maxBframe", &MaxBframe);
    }
    if (frameRate) {
        if (PyTuple_Check(frameRate) && PyTuple_Size(frameRate) == 2) {
            Self->_in_Handle->setParameter("frameRate", frameRate);
        }
        else {
            cerr << "Warning.setParameter: {frameRate} must be a 2-dim tuple, so there is no valid update in this step." << endl;
        }
    }
    temp_str.clear();
    PyBuffer_Release(&videoPath);
    PyBuffer_Release(&codecName);
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_setParam(C_MpegClient* Self, PyObject *args, PyObject *kwargs) {
    /* ��װ(void)C_MPCT_setParam��������������Ϊ:
    *   parameter [byte->buffer]: ��Ҫ��ȡ�Ĳ���
    *       widthDst/heightDst: [int]   Ŀ��֡��/��
    *       cacheSize/readSize: [int]   ����/��ȡ��ģ
    *       dstFrameRate:       [tuple] ֡��
    */
    int widthDst = 0;
    int heightDst = 0;
    int64_t cacheSize = 0;
    int64_t readSize = 0;
    PyObject *frameRate = nullptr;
    static char *kwlist[] = { "widthDst", "heightDst", "cacheSize", "readSize", "dstFrameRate", nullptr };
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|iiLLO", kwlist, &widthDst, &heightDst, &cacheSize, &readSize, &frameRate)) {
        PyErr_SetString(PyExc_TypeError, "Error.FFmpegSetup: need 'params'");
        return nullptr;
    }
    if (widthDst>0) {
        Self->_in_Handle->setParameter("widthDst", &widthDst);
    }
    if (heightDst>0) {
        Self->_in_Handle->setParameter("heightDst", &heightDst);
    }
    if (cacheSize>0) {
        Self->_in_Handle->setParameter("cacheSize", &cacheSize);
    }
    if (readSize>0) {
        Self->_in_Handle->setParameter("readSize", &readSize);
    }
    if (frameRate) {
        if (PyTuple_Check(frameRate) && PyTuple_Size(frameRate) == 2) {
            Self->_in_Handle->setParameter("dstFrameRate", frameRate);
        }
        else {
            cerr << "Warning.setParameter: {dstFrameRate} must be a 2-dim tuple, so there is no valid update in this step." << endl;
        }
    }
    Py_RETURN_NONE;
}

static PyObject* C_MPDC_DumpFile(C_MpegDecoder* Self) {
    /* ��װ(void)dumpFormat���������������� */
    Self->_in_Handle->dumpFormat();
    Py_RETURN_NONE;
}

static PyObject* C_MPEC_DumpFile(C_MpegEncoder* Self) {
    /* ��װ(void)dumpFormat���������������� */
    Self->_in_Handle->dumpFormat();
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_DumpFile(C_MpegClient* Self) {
    /* ��װ(void)dumpFormat���������������� */
    Self->_in_Handle->dumpFormat();
    Py_RETURN_NONE;
}

static PyObject* C_MPDC_Clear(C_MpegDecoder* Self) {
    /* ��װ(void)clear���������������� */
    Self->_in_Handle->clear();
    Py_RETURN_NONE;
}

static PyObject* C_MPEC_Clear(C_MpegEncoder* Self) {
    /* ��װ(void)clear���������������� */
    Self->_in_Handle->clear();
    Py_RETURN_NONE;
}

static PyObject* C_MPCT_Clear(C_MpegClient* Self) {
    /* ��װ(void)clear���������������� */
    Self->_in_Handle->clear();
    Py_RETURN_NONE;
}

static PyObject* C_MPEC_Close(C_MpegEncoder* Self) {
    /* ��װ(void)close���������������� */
    Self->_in_Handle->FFmpegClose();
    Py_RETURN_NONE;
}

/*****************************************************************************
* ����ģ���¼��ע��
*****************************************************************************/
static PyMethodDef C_MPC_MethodMembers[] =      //ע��ȫ�ֺ����б�
{
    { "setGlobal",       (PyCFunction)C_MPC_Global,             METH_VARARGS | METH_KEYWORDS, \
    "Set global setting parameters.\n - dumpLevel: [int] the level of dumped log.\n   -|- 0: silent executing.\n   -|- 1: [default] dump basic informations.\n   -|- 2: dump all informations." },
    { "readme",          (PyCFunction)C_MPC_Help,               METH_NOARGS, \
    "Use it to see readme and some useful instructions." },
    { nullptr, nullptr, 0, nullptr }
};

static PyMethodDef C_MPDC_MethodMembers[] =      //ע��������г�Ա�����ṹ�б�.
{ //�ò������弴Ϊ��һ����װCMISS_Handle��Ϊ���ṩ����Python�Ľӿ�
    { "ResetPath",          (PyCFunction)C_MPDC_resetPath,         METH_VARARGS | METH_KEYWORDS, \
    "Reset the path of decoded video.\n - videoPath: [bytes] the path of decoded video file." },
    { "FFmpegSetup",        (PyCFunction)C_MPDC_Setup,             METH_VARARGS | METH_KEYWORDS, \
    "Reset the decoder and the video format.\n - videoPath: [bytes] the path of decoded video file." },
    { "ExtractFrame",       (PyCFunction)C_MPDC_ExtractFrame,      METH_VARARGS | METH_KEYWORDS, \
    "Extract a series of continius frames at the specific position.\n - framePos: [int] the start position of the decoder.\n - frameNum: [int] the expected number of extracted frames." },
    { "ExtractFrameByTime", (PyCFunction)C_MPDC_ExtractFrame_Time, METH_VARARGS | METH_KEYWORDS, \
    "Extract a series of continius frames at the specific position (time based).\n - timePos: [double] the start position (second) of the decoder.\n - frameNum: [int] the expected number of extracted frames." },
    { "ExtractGOP",         (PyCFunction)C_MPDC_ExtractGOP,        METH_VARARGS | METH_KEYWORDS, \
    "Extract a series of continius frames as a GOP at the specific position.\n - framePos: [int] the start position of the decoder." },
    { "ExtractGOPByTime",   (PyCFunction)C_MPDC_ExtractGOP_Time,   METH_VARARGS | METH_KEYWORDS, \
    "Extract a series of continius frames as a GOP at the specific position (time based).\n - timePos: [double] the start position (second) of the decoder." },
    { "ResetGOPPosition",   (PyCFunction)C_MPDC_setGOPPosition,    METH_VARARGS | METH_KEYWORDS, \
    "Reset the start position of GOP flow.\n - framePos: [int] the start position of the decoder.\n - timePos: [double] the start position (second) of the decoder." },
    { "clear",              (PyCFunction)C_MPDC_Clear,             METH_NOARGS, \
    "Clear all states (except the videoPath)." },
    { "dumpFile",           (PyCFunction)C_MPDC_DumpFile,          METH_NOARGS, \
    "Show current state of formatContex." },
    { "setParameter",       (PyCFunction)C_MPDC_setParam,          METH_VARARGS | METH_KEYWORDS, \
    "Set the optional parameters of 'Setup' & 'Extract' functions via different methods.\n - widthDst: [int] the width of destination (frame), if <=0 (default), it would take no effect.\n - heightDst: [int] the height of destination (frame), if <=0 (default), it would take no effect." },
    { "getParameter",       (PyCFunction)C_MPDC_getParam,          METH_VARARGS | METH_KEYWORDS, \
    "Input a parameter's name to get it.\n - paramName: [bytes] the name of needed parameter.\n   -|- videoPath: [bytes] the current path of the read video.\n   -|- width/height: [int] the size of one frame.\n   -|- frameCount: [int] the number of returned frames in the last ExtractFrame().\n   -|- coderName: [bytes] the name of the decoder.\n   -|- duration: [double] the total seconds of this video.\n   -|- estFrameNum: [int] the estimated total frame number(may be not accurate).\n   -|- avgFrameRate: [double] the average of FPS." },
    { nullptr, nullptr, 0, nullptr }
};

static PyMethodDef C_MPEC_MethodMembers[] =      //ע��������г�Ա�����ṹ�б�.
{ //�ò������弴Ϊ��һ����װCMISS_Handle��Ϊ���ṩ����Python�Ľӿ�
    { "FFmpegSetup",        (PyCFunction)C_MPEC_Setup,             METH_VARARGS | METH_KEYWORDS, \
    "Open the encoded video and reset the encoder.\n - videoPath: [bytes] the path of encoded(written) video file." },
    { "EncodeFrame",        (PyCFunction)C_MPEC_EncodeFrame,       METH_VARARGS | METH_KEYWORDS, \
    "Encode one frame.\n - PyArrayFrame: [ndarray] the frame that needs to be encoded." },
    { "setParameter",       (PyCFunction)C_MPEC_setParam,          METH_VARARGS | METH_KEYWORDS, \
    "Set the necessary parameters of 'Setup' & 'Encode' functions via different methods.\n - decoder: [MpegDecoder] copy metadata from a known decoder.\n - videoPath: [bytes] the current path of the encoded video.\n - codecName: [bytes] the name of the encoder.\n - bitRate: [float] the indended bit rate (Kb/s).\n - width/height: [int] the size of one (scaled) frame.\n - widthSrc/heightSrc: [int] the size of one input frame, if set <=0, these parameters would not be enabled.\n - GOPSize: [int] the number of frames in a GOP.\n - maxBframe: [int] the maximal number of B frames in a GOP.\n - frameRate: [tuple] a 2-dim tuple indicating the FPS(num, den) of the stream." },
    { "clear",              (PyCFunction)C_MPEC_Clear,             METH_NOARGS, \
    "Clear all states." },
    { "dumpFile",           (PyCFunction)C_MPEC_DumpFile,          METH_NOARGS, \
    "Show current state of formatContex." },
    { "FFmpegClose",        (PyCFunction)C_MPEC_Close,             METH_NOARGS, \
    "Close currently encoded video and write the end code of a MPEG file." },
    { nullptr, nullptr, 0, nullptr }
};

static PyMethodDef C_MPCT_MethodMembers[] =      //ע��������г�Ա�����ṹ�б�.
{ //�ò������弴Ϊ��һ����װCMISS_Handle��Ϊ���ṩ����Python�Ľӿ�
    { "ResetPath",          (PyCFunction)C_MPCT_resetPath,         METH_VARARGS | METH_KEYWORDS, \
    "Reset the address of decoded video.\n - videoAddress: [bytes] the path of decoded video file." },
    { "FFmpegSetup",        (PyCFunction)C_MPCT_Setup,             METH_VARARGS | METH_KEYWORDS, \
    "Reset the decoder and the video format.\n - videoAddress: [bytes] the path of decoded video file." },
    { "start",              (PyCFunction)C_MPCT_Start,             METH_NOARGS, \
    "Start the demuxing thread, must be called after FFmpegSetup()." },
    { "terminate",           (PyCFunction)C_MPCT_Terminate,        METH_NOARGS, \
    "Terminate all current demuxing threads, usually used when there is only one thread." },
    { "ExtractFrame",       (PyCFunction)C_MPCT_ExtractFrame,      METH_VARARGS | METH_KEYWORDS, \
    "Extract frames from the current buffer.\n - readSize: [int] the number of extracted frames, should not be larger than cache number. \nIf not set, will be used as the default value." },
    { "clear",              (PyCFunction)C_MPCT_Clear,             METH_NOARGS, \
    "Clear all states (except the videoAddress)." },
    { "dumpFile",           (PyCFunction)C_MPCT_DumpFile,          METH_NOARGS, \
    "Show current state of formatContex." },
    { "setParameter",       (PyCFunction)C_MPCT_setParam,          METH_VARARGS | METH_KEYWORDS, \
    "Set the optional parameters of 'Setup' & 'Extract' functions and the demuxing thread via different methods.\n - widthDst: [int] the width of destination (frame), if <=0 (default), it would take no effect.\n - heightDst: [int] the height of destination (frame), if <=0 (default), it would take no effect.\n - cacheSize: [int] the number of allocated avaliable frames in the cache.\n - readSize: [int] the default value of ExtractFrame().\n - dstFrameRate: [tuple] a 2-dim tuple indicating the destination FPS(num, den) of the stream." },
    { "getParameter",       (PyCFunction)C_MPCT_getParam,          METH_VARARGS | METH_KEYWORDS, \
    "Input a parameter's name to get it.\n - paramName: [bytes] the name of needed parameter.\n   -|- videoPath: [bytes] the current path of the read video.\n   -|- width/height: [int] the size of one frame.\n   -|- frameCount: [int] the number of returned frames in the last ExtractFrame().\n   -|- coderName: [bytes] the name of the decoder.\n   -|- duration: [double] the total seconds of this video.\n   -|- estFrameNum: [int] the estimated total frame number(may be not accurate).\n   -|- srcFrameRate: [double] the average of FPS of the source video." },
    { nullptr, nullptr, 0, nullptr }
};


/*****************************************************************************
* ��/�ṹ�����г�Ա���������Ե�˵����Ϣ..
* ΪPython���ṩ����ķ�װ
*****************************************************************************/
static PyTypeObject C_MPDC_ClassInfo =
{
    PyVarObject_HEAD_INIT(nullptr, 0)"mpegCoder.MpegDecoder",  //����ͨ��__class__�������ַ���. CPP��������.__name__��ȡ.
    sizeof(C_MpegDecoder),                 //��/�ṹ�ĳ���.����PyObject_Newʱ��Ҫ֪�����С.
    0,
    (destructor)C_MPDC_Destruct,    //�����������.
    0,
    0,
    0,
    0,
    (reprfunc)C_MPDC_Repr,
    0,
    0,
    0,
    0,
    0,
    (reprfunc)C_MPDC_Str,         //Str/print���ú�������.
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,     //���û���ṩ�����Ļ���ΪPy_TPFLAGS_DEFAULE
    "This class has wrapped the C-API of FFmpeg decoder so that users could call its methods\n to decode the frame data in python quickly.",   //__doc__,��/�ṹ��DocString.
    0,
    0,
    0,
    0,
    0,
    0,
    C_MPDC_MethodMembers,       //������з�������.
    C_MPDC_DataMembers,         //����������ݳ�Ա����.
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)C_MPDC_init,      //��Ĺ��캯��.
    0,
};

static PyTypeObject C_MPEC_ClassInfo =
{
    PyVarObject_HEAD_INIT(nullptr, 0)"mpegCoder.MpegEncoder",  //����ͨ��__class__�������ַ���. CPP��������.__name__��ȡ.
    sizeof(C_MpegEncoder),                 //��/�ṹ�ĳ���.����PyObject_Newʱ��Ҫ֪�����С.
    0,
    (destructor)C_MPEC_Destruct,    //�����������.
    0,
    0,
    0,
    0,
    (reprfunc)C_MPEC_Repr,
    0,
    0,
    0,
    0,
    0,
    (reprfunc)C_MPEC_Str,         //Str/print���ú�������.
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,     //���û���ṩ�����Ļ���ΪPy_TPFLAGS_DEFAULE
    "This class has wrapped the C-API of FFmpeg encoder so that users could call its methods\n to encode frames by using numpy-data quickly.",   //__doc__,��/�ṹ��DocString.
    0,
    0,
    0,
    0,
    0,
    0,
    C_MPEC_MethodMembers,       //������з�������.
    C_MPEC_DataMembers,         //����������ݳ�Ա����.
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)C_MPEC_init,      //��Ĺ��캯��.
    0,
};

static PyTypeObject C_MPCT_ClassInfo =
{
    PyVarObject_HEAD_INIT(nullptr, 0)"mpegCoder.MpegClient",  //����ͨ��__class__�������ַ���. CPP��������.__name__��ȡ.
    sizeof(C_MpegClient),                 //��/�ṹ�ĳ���.����PyObject_Newʱ��Ҫ֪�����С.
    0,
    (destructor)C_MPCT_Destruct,    //�����������.
    0,
    0,
    0,
    0,
    (reprfunc)C_MPCT_Repr,
    0,
    0,
    0,
    0,
    0,
    (reprfunc)C_MPCT_Str,         //Str/print���ú�������.
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,     //���û���ṩ�����Ļ���ΪPy_TPFLAGS_DEFAULE
    "This class has wrapped the C-API of FFmpeg demuxer so that users could call its methods\n to demux the network stream in python quickly.",   //__doc__,��/�ṹ��DocString.
    0,
    0,
    0,
    0,
    0,
    0,
    C_MPCT_MethodMembers,       //������з�������.
    C_MPCT_DataMembers,         //����������ݳ�Ա����.
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)C_MPCT_init,      //��Ĺ��캯��.
    0,
};

/*****************************************************************************
* ��ģ��˵����Ϣ..
* ΪPythonģ���ṩ����ķ�װ
*****************************************************************************/
static PyModuleDef ModuleInfo =
{
    PyModuleDef_HEAD_INIT,
    "mpegCoder",               //ģ���������--__name__.
    "A FFmpeg module which could provide a class for encode/decode a video in any format.",  //ģ���DocString.__doc__
    -1,
    nullptr, nullptr, nullptr, nullptr, nullptr
};

#endif