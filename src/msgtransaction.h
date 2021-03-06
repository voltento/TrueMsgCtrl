#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "msgtemplates.h"
#include <algorithm>
#include <cstring>
#include <assert.h>

class MsgTransaction
{
public:
    MsgTransaction(char* id = nullptr, unsigned int idSize = 0):
        id(nullptr),
        idSize(idSize)
    {
        setId(id);
    }
    ~MsgTransaction()
    {
        if(respData)
            free(respData);
        if(reqData)
            free(reqData);
        if(id)
            free(id);
    }

    const char* getId()const{return id;}
    const char* getRespDataWithId()
    {
        if(respData) return respData;
        return getDataWithId(resp,respData);
    }
    const char* getRespData()
    {
        if(resp)
            return resp->getData();
        if(respData)
            return respData + idSize;
        assert(false);
    }
    const char* getReqData()
    {
        if(req)
            return req->getData();
        if(reqData)
            return reqData + idSize;
        assert(false);
    }

    const char* getReqDataWithId()
    {
        return getDataWithId(req,reqData);
    }


    const char* setId(const char* data)
    {
        if(idSize)
        {
            id = (char*)realloc(id,idSize);
            memcpy(id,data,idSize);
        }
        return data+idSize;
    }
    bool checkId(char* id)
    {
        assert(idSize);
        return (0 == memcmp(this->id,id,idSize));
    }

    void setReq(AMsgType *value){req = value;}

    void setResp(AMsgType *value){resp = value;}
    void setReq(const char *value, unsigned int size)
    {
        reqData = setData(value,size);
        reqDataSize = size +idSize;
    }

    void setResp(const char *value, unsigned int size)
    {
        respData = setData(value,size);
        respDataSize = size +idSize;
    }

    AMsgType *getReq() const
    {
        return req;
    }

    AMsgType *getResp() const
    {
        return resp;
    }

    unsigned int getReqSize()
    {
        if(req)
            return sizeof(*req);
        else return reqDataSize;
    }
    unsigned int getRespSize()
    {
        if(resp)
            return sizeof(*resp);
        else return respDataSize;
    }

    void setIdSize(unsigned int size)
    {
        idSize = size;
    }

    unsigned int getIdSize()const{return idSize;}
private:
    const char* getDataWithId(AMsgType* src, char*& dest)
    {
        if(idSize)
        {
            dest = (char*)malloc(src->getSize()+idSize);
            memcpy(dest,id,idSize);
            memcpy(dest+idSize,src->getData(),src->getSize());
            return dest;
        }
        else
            return src->getData();
    }

    char* setData(const char *src, unsigned int size)
    {
        char* res = (char*)malloc(size+idSize);
        memcpy(res,id,idSize);
        memcpy(res+idSize,src,size);
        return res;
    }


    AMsgType* req = nullptr;
    AMsgType* resp = nullptr;
    char* id = nullptr;
    unsigned int idSize = 0;
    char* respData = nullptr;
    char* reqData = nullptr;
    unsigned int respDataSize = 0;
    unsigned int reqDataSize = 0;
};




#endif // TRANSACTION_H




