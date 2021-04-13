
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
#pragma comment(lib, "ws2_32.lib")

#include <simstruc.h>
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 12

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
/* extern double func(double a); */
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Start function
 *
 */
void drone_tcp_send_Start_wrapper(void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1)
{
/* %%%-SFUNWIZ_wrapper_Start_Changes_BEGIN --- EDIT HERE TO _END */
// malloc pWork memory for socket
    pW[0] = malloc(sizeof(SOCKET *));
    
    int iResult;
    // winsock init
    WSADATA wsadata;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iResult != 0) {
        ssPrintf("WSAStartip failed: %d\n", iResult);
        return;
    }
    // check version
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
        ssPrintf("socket version not match!\n");
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
    ssPrintf("server info %s:%s\n", serverAddr, serverPort);
    
    iResult = getaddrinfo(serverAddr, serverPort, &hints, &result);
    if (iResult != 0) {
        ssPrintf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }
    // create a SOCKET for connecting to server
    ptr = result;
    *pSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (*pSock == INVALID_SOCKET) {
        ssPrintf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }
    // set timeout
    int recvTimeout = 1 * 1000; // 1s
    int sendTimeout = 1 * 10000; // 1s
    setsockopt(*pSock, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvTimeout, sizeof(int));
    setsockopt(*pSock, SOL_SOCKET, SO_SNDTIMEO, (char *)&sendTimeout, sizeof(int));

    // connect to server
    iResult = connect(*pSock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(*pSock);
        *pSock = INVALID_SOCKET;
        ssPrintf("socket connect FAILED!\n");
    }
    else {
        ssPrintf("socket connect SUCCESS!\n");
    }
    
    freeaddrinfo(result);
    if (*pSock == INVALID_SOCKET) {
        ssPrintf("Unable to connect to server!\n");
        WSACleanup();
        return;
    }

    // role message send
    typedef struct {
        int32_T id;
        int32_T type; // 0 for recv(posture from commu server to target drone client)
                      // 1 for send (drone to commu server)
    } Role;
    const int ROLE_SIZE = sizeof(Role);
    Role send_role = {666, 1};
    iResult = send(*pSock, (char *)&send_role, ROLE_SIZE, 0);
    if (iResult == -1) {
        ssPrintf("[tcp_send] send role fail: %d\n", WSAGetLastError());
    }
/* %%%-SFUNWIZ_wrapper_Start_Changes_END --- EDIT HERE TO _BEGIN */
}
/*
 * Output function
 *
 */
void drone_tcp_send_Outputs_wrapper(const int32_T *u0,
			const int8_T *received,
			int32_T *y0,
			void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
// define struct
    typedef struct {
        int32_T x;
        int32_T y;
        int32_T z;
        int32_T u;
        int32_T v;
        int32_T w;
        int32_T vx;
        int32_T vy;
        int32_T vz;
        int32_T vu;
        int32_T vv;
        int32_T vw;
    } Posture;
    const int POSTURE_SIZE = sizeof(Posture);
    ���ı��� = 1;
    if (*received == 0) {
        ssPrintf("[tcp_send] not received, not send\n");
        return;
    }

    // retrieve TCP Socket from pWork
    SOCKET *pSock = (SOCKET *)pW[0];
    int iResult;

    // build posture
    // Posture buffer = {u0[0], u0[1], u0[2], u0[3], u0[4], u0[5]};
    
    Posture send_buf;
    memcpy((char *)&send_buf, u0, POSTURE_SIZE);
    ssPrintf("[tcp_send] will send x = %d, y = %d, z = %d\n", send_buf.x, send_buf.y, send_buf.z);
    // send by socket
    iResult = send(*pSock, (char *)&send_buf, POSTURE_SIZE, 0);
    ssPrintf("[tcp_send] send bytes: %d\n\n", iResult);
    if (iResult == -1) {
        ssPrintf("[tcp_send] Error at socket : %ld\n", WSAGetLastError());
    }

    // output
    memcpy(y0, (char *)&send_buf, POSTURE_SIZE);
    ssPrintf("-------------------------\n");

/*
    y0[0] = recv_buf.x;
    y0[1] = recv_buf.y;
    y0[2] = recv_buf.z;
    y0[3] = recv_buf.u;
    y0[4] = recv_buf.v;
    y0[5] = recv_buf.w;
*/
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
 * Terminate function
 *
 */
void drone_tcp_send_Terminate_wrapper(void **pW,
			const uint8_T *para_addr, const int_T p_width0,
			const int32_T *para_port, const int_T p_width1)
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

