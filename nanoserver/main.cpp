#include <QCoreApplication>
#include"nn.h"
#include"pair.h"
#include<string>


#define SOCKET_ADDRESS1 "ipc://test"
#define SOCKET_ADDRESS2 "tcp://127.0.0.1:7766"
#define SOCKET_ADDRESS3 "tcp://*:7766"
#define HELLOWORLD "Hello world!2222222"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int rc = 0;
        int pair_socket = 0;
        int str_len = 0;
        char buf[256] = { 0 };
        char buf1[256] = { 0 };
        int timeo = 5000;
        //���㳤��
        str_len = strlen(HELLOWORLD);
        //��ʼ��socket
        pair_socket = nn_socket(1, NN_PAIR);
        if (pair_socket == -1) {
            printf("nn_socket failed! error.\n");
            system("pause");
            return 0;
        }
        //���ó�ʱ
        rc = nn_setsockopt(pair_socket, 0, NN_SNDTIMEO, &timeo, sizeof(timeo));
        rc = nn_setsockopt(pair_socket, 0, NN_RCVTIMEO, &timeo, sizeof(timeo));
        //�󶨶˿�
        rc = nn_bind(pair_socket, SOCKET_ADDRESS1);
        if (rc < 0) {
            printf("bind failed! error\n");
            system("pause");
            return ETIMEDOUT;
        }
        //��hello world���Ƶ�buf��
        memcpy(buf1, HELLOWORLD, str_len);
        while (1) {
            //�ȴ���������
            rc = nn_recv(pair_socket, buf, 256, 0);
            if (rc < 0) {
                printf("nn_recv failed! error\n");
                continue;
            }
            //��ӡ
            printf("recv:%s\n", buf);
            memset(buf, 0, 256);
            //������Ҫ�ǲ���ʹ�ã�ƽʱ��Ŀ��Ҫʹ�ñ�ǩ
        send_test:
            //��������
            rc = nn_send(pair_socket, buf1, str_len, 0);
            if (rc < 0) {
                printf("nn_recv failed! error");
                goto send_test;//������Ҫ�ǲ���ʹ�ã�ƽʱ��Ŀ��Ҫʹ�ñ�ǩ
            }
            printf("send:%s\n", buf1);
        }
        //�ر��׽���
        rc = nn_close(pair_socket);
        if (rc != 1) {
            printf("nn_close failed! error");
            system("pause");
            return 0;
        }


    return a.exec();
}
