//
//
#include "CEncDesProtocol.h"
int CEncDesProtocol::encode_file(const char *fileInName, const char *fileOutName) {
    int ret = 0;
    unsigned char inbuf[1024 * 4] = {0}, outbuf[1024 * 4] = {0};
    int inlen = 0, outlen = 0, donelen = 0;

    if (fileInName == NULL || fileOutName == NULL) {
        ret = -1;
        cout << __FILE__ << "in line " << __LINE__ << "NULL pointer error: " << ret;
        return ret;
    }
    FILE *fp1 = fopen(fileInName, "rb");
    FILE *fp2 = fopen(fileOutName, "wb");
    if (fp1 == NULL || fp2 == NULL) {
        ret = -2;
        cout << __FILE__ << "in line " << __LINE__ << "open file error: " << ret;
        goto END;
    }
    while (!feof(fp1)) {
        memset(inbuf, 0, 4 * 1024);
        inlen = fread(inbuf, 1, SIZE, fp1);
        if (inlen < SIZE)
            break;

        DesEnc_raw(inbuf, inlen, outbuf, &outlen);
        //strcpy((char*)outbuf, (char*)inbuf);
        donelen = fwrite(outbuf, 1, outlen, fp2);
        if (outlen != donelen) {
            ret = -3;
            cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
            goto END;
        }

    }
    DesEnc(inbuf, inlen, outbuf, &outlen);
    //strcpy((char*)outbuf, (char*)inbuf);
    donelen = fwrite(outbuf, 1, outlen, fp2);
    if (outlen != donelen) {
        ret = -3;
        cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
        goto END;
    }

    END:
    if (fp1)
        fclose(fp1);
    if (fp2)
        fclose(fp2);
    return ret;
}

int CEncDesProtocol::decode_file(const char *fileInName, const char *fileOutName) {
    int ret = 0;
    int inlen = 0, outlen = 0, donelen = 0;
    unsigned char inbuf[SIZE] = {0};
    unsigned char outbuf[SIZE] = {0};

    if (fileInName == NULL || fileOutName == NULL) {
        ret = -1;
        cout << __FILE__ << "in line " << __LINE__ << "NULL pointer error: " << ret;
        return ret;
    }
    FILE *fp1 = fopen(fileInName, "rb");
    FILE *fp2 = fopen(fileOutName, "wb");
    if (fp1 == NULL || fp2 == NULL) {
        ret = -2;
        cout << __FILE__ << "in line " << __LINE__ << "open file error: " << ret;
        goto END;
    }
    while (!feof(fp1)) {
        inlen = fread(inbuf, 1, SIZE, fp1);
        if (inlen < SIZE)
            break;
        DesDec_raw(inbuf, inlen, outbuf, &outlen);
        donelen = fwrite(outbuf, 1, outlen, fp2);
        if (donelen != outlen) {
            ret = -3;
            cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
            goto END;

        }
    }
    DesDec(inbuf, inlen, outbuf, &outlen);
    donelen = fwrite(outbuf, 1, outlen, fp2);
    if (donelen != outlen) {
        ret = -3;
        cout << __FILE__ << "in line " << __LINE__ << "write file error: " << ret;
        goto END;
    }
    END:
    if (fp1)
        fclose(fp1);
    if (fp2)
        fclose(fp2);
    return ret;
}

