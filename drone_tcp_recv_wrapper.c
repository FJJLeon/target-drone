
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#include <simstruc.h>
#include <drone_util.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define y_width 19

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
 
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Start function
 *
 */
void drone_tcp_recv_Start_wrapper(void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1,
			const int32_T *para_role_type, const int_T p_width2,
			const int32_T *para_role_tag, const int_T p_width3,
			const int32_T *para_role_id, const int_T p_width4)
{
/* %%%-SFUNWIZ_wrapper_Start_Changes_BEGIN --- EDIT HERE TO _END */
// malloc pWork memory for filename      
    
    pW[1] = getLogName(para_role_id[0], para_role_tag[0]);
    
    const char *fn = (const char*) pW[1];
    LOG(DEBUG, 1, fn, "LOG FILE: %s\n", fn);
    
    
    // malloc pWork memory for socket
    pW[0] = malloc(sizeof(SOCKET *));
    
    int iResult;
    // winsock init
    WSADATA wsadata;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        LOG(ERROR, 1, fn, "WSAStartip failed: %d\n", iResult);
        return;
    }
    // check version
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        LOG(ERROR, 1, fn, "socket version not match!\n");
        WSACleanup();
    }
    // store SOCKET object into pWork
    SOCKET* pSock = (SOCKET *)pW[0];
    *pSock = INVALID_SOCKET;
    // ddeclare addrinfo that contains a sockaddr structure
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    // resolve the server address and port
    char serverAddr[50];
    char serverPort[10];
    sprintf(serverAddr, "%d.%d.%d.%d", para_addr[0], para_addr[1], para_addr[2], para_addr[3]);
    sprintf(serverPort, "%d", para_port[0]);
    LOG(DEBUG, 1, fn, "server info %s:%s\n", serverAddr, serverPort);
    
    iResult = getaddrinfo(serverAddr, serverPort, &hints, &result);
    if (iResult != 0) {
        LOG(ERROR, 1, fn, "getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }
    // create a SOCKET for connecting to server
    ptr = result;
    *pSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (*pSock == INVALID_SOCKET) {
        LOG(ERROR, 1, fn, "Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }
    // set timeout
    int recvTimeout = 1 * 3000;  // ms
    int sendTimeout = 1 * 10000; // ms
    setsockopt(*pSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(int));
    setsockopt(*pSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&sendTimeout, sizeof(int));

    // connect to server
    iResult = connect(*pSock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(*pSock);
        *pSock = INVALID_SOCKET;
        LOG(ERROR, 1, fn, "recv socket connect FAILED!\n");
    }
    else {
        LOG(DEBUG, 1, fn, "recv socket connect SUCCESS!\n");
    }
    
    freeaddrinfo(result);
    if (*pSock == INVALID_SOCKET) {
        LOG(ERROR, 1, fn, "Unable to connect to server!\n");
        WSACleanup();
        return;
    }

    Role recv_role = {para_role_type[0], para_role_tag[0], para_role_id[0], ROLE_DIR_RECV};
    iResult = send(*pSock, (char *)&recv_role, ROLE_SIZE, 0);
    if (iResult == -1) {
        LOG(ERROR, 1, fn, "send role fail: %d\n", WSAGetLastError());
    }
/* %%%-SFUNWIZ_wrapper_Start_Changes_END --- EDIT HERE TO _BEGIN */
}
/*
 * Output function
 *
 */
void drone_tcp_recv_Outputs_wrapper(int32_T *out,
			int8_T *received,
			void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1,
			const int32_T *para_role_type, const int_T p_width2,
			const int32_T *para_role_tag, const int_T p_width3,
			const int32_T *para_role_id, const int_T p_width4)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
// default received flag
    *received = 1;    
    
    // retrieve TCP Socket from pWork
    SOCKET *pSock = (SOCKET *)pW[0];
    // retrieve LOG filename from pWork
    const char *fn = (const char *)pW[1];
    
    int iResult;

    // recv posture buffer
    RecvPackage recv_buf;
    memset((char*)&recv_buf, 0, RECVPACKAGE_SIZE);
    // recv by socket
    iResult = recv(*pSock, (char*)&recv_buf, RECVPACKAGE_SIZE, 0);
    if (iResult == -1) {
        LOG(WARNING, 1, fn, "Error at socket@%d: %ld\n", para_port[0], WSAGetLastError());
        // TODO, if socket not connect in Start, here error code = 10038 and no timeout wait
        *received = 0;
        return;
    }
    LOG(DEBUG, 1, fn, "recv bytes %d\n", iResult);
    LOG(DEBUG, 1, fn, "recv posture x = %d, y = %d, z = %d\n", recv_buf.post.x, recv_buf.post.y, recv_buf.post.z);
    // output
    memcpy(out, (char*)&recv_buf, RECVPACKAGE_SIZE);
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
 * Terminate function
 *
 */
void drone_tcp_recv_Terminate_wrapper(void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1,
			const int32_T *para_role_type, const int_T p_width2,
			const int32_T *para_role_tag, const int_T p_width3,
			const int32_T *para_role_id, const int_T p_width4)
{
/* %%%-SFUNWIZ_wrapper_Terminate_Changes_BEGIN --- EDIT HERE TO _END */
// retrieve TCP Socket from pWork
    SOCKET *pSock = (SOCKET *)pW[0];
    // close socket and free memory
    closesocket(*pSock);
    free(pW[0]);
    WSACleanup();
/* %%%-SFUNWIZ_wrapper_Terminate_Changes_END --- EDIT HERE TO _BEGIN */
}

