#include "nf_signal.h"
#include "string.h"

#include "nf_assert.h"

/* 信号对象类型 */
typedef struct _nf_sig
{
    NF_SignalName  Name;
    NF_SignalValue Value;
}NF_Signal;

/* 存储信号的表 */
static NF_Signal NF_Signal_SigList[NF_MAX_SIGNAL_NUM] = {0};
static NF_Int32U NF_Signal_SigListCnt = 0;


/* 按信号名在表中查找信号对象 */
static NF_Signal* NF_Signal_Search(NF_SignalName name, NF_SignalID* id_ret)
{
    NF_Int32U i;

    /* 信号名不能为空 */
    NF_ASSERT( name != NF_NULL_PTR );

    for (i = 0; i < NF_Signal_SigListCnt; i ++)
    {
        if ( strcmp( NF_Signal_SigList[i].Name, name ) == 0 )
        {
            /* 通过回调参数返回ID号 */
            if (id_ret != NF_NULL_PTR)
            {
                *id_ret = i;
            }

            return &(NF_Signal_SigList[i]);
        }
    }

    /* 没有找到，返回空指针 */
    return NF_NULL_PTR;
}

/* 创建一个新信号 */
static NF_Signal* NF_Signal_Create(NF_SignalName name, NF_SignalValue val, NF_SignalID* id_ret)
{
    /* 信号名不能为空 */
    NF_ASSERT( name != NF_NULL_PTR );
    
    if (NF_Signal_SigListCnt < NF_MAX_SIGNAL_NUM)
    {
        NF_Signal_SigList[NF_Signal_SigListCnt].Name  = name;
        NF_Signal_SigList[NF_Signal_SigListCnt].Value = val;
        NF_Signal_SigListCnt ++;

        /* 通过回调参数返回ID号 */
        if (id_ret != NF_NULL_PTR)
        {
            *id_ret = NF_Signal_SigListCnt - 1;
        }
        
        return &(NF_Signal_SigList[NF_Signal_SigListCnt - 1]);
    }
    else
    {
        /* 信号列表满，创建失败 */
        /* 此时应加大头文件中最大信号数量宏NF_MAX_SIGNAL_NUM */
        return NF_NULL_PTR;
    }
}


/* 通过字符串名称给赋值信号，如不存在则创建，时间复杂度最高O(n) */
NF_SignalID NF_Signal_Set(NF_SignalName name, NF_SignalValue val)
{
    NF_Signal* sig = NF_NULL_PTR;
    NF_SignalID id;

    /* 信号名不能为空 */
    NF_ASSERT( name != NF_NULL_PTR );
    
    sig = NF_Signal_Search(name, &id);
    if (sig != NF_NULL_PTR)
    {
        sig->Value = val;
        return id;
    }
    else
    {
        sig = NF_Signal_Create(name, val, &id);

        /* 断言失败则信号列表满，需增大NF_MAX_SIGNAL_NUM */
        NF_ASSERT( sig != NF_NULL_PTR );

        return id;
    }
}

/* 通过创建信号时获得的ID号赋值信号，如不存在则无效，时间复杂度O(1) */
void NF_Signal_SetID(NF_SignalID id, NF_SignalValue val)
{
    /* ID号需有效 */
    NF_ASSERT( id < NF_Signal_SigListCnt );

    if (id < NF_Signal_SigListCnt)
    {
        NF_Signal_SigList[id].Value = val;
    }
}

/* 通过字符串名称获取信号值，如不存在返回0，时间复杂度最高O(n) */
NF_SignalValue NF_Signal_Get(NF_SignalName name)
{
    NF_Signal* sig = NF_NULL_PTR;

    /* 信号名不能为空 */
    NF_ASSERT( name != NF_NULL_PTR );
    
    sig = NF_Signal_Search(name, NF_NULL_PTR);

    if (sig != NF_NULL_PTR)
    {
        /* 搜索成功返回信号值 */
        return sig->Value;
    }
    else
    {
        /* 搜索失败返回0 */
        return 0;
    }
}

/* 通过创建信号时获得的ID获取信号值，如不存在返回0，时间复杂度O(1) */
NF_SignalValue NF_Signal_GetID(NF_SignalID id)
{
    /* ID号需有效 */
    NF_ASSERT( id < NF_Signal_SigListCnt );

    if (id < NF_Signal_SigListCnt)
    {
        return NF_Signal_SigList[id].Value;
    }
	else
	{
		return 0;
	}
}
