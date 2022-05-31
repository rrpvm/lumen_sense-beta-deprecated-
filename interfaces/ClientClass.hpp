#pragma once
#include "../utils/singleton.h"
class RecvProp;

class RecvTable
{
public:
	RecvProp* m_pProps;//RecvProp*
	int				m_nProps;
	void* m_pDecoder;
	const char* m_pNetTableName;
	bool			m_bInitialized;
	bool			m_bInMainList;
};

class RecvProp
{
public:

	const char* m_pVarName;
	int  m_RecvType;
	int				m_Flags;
	int				m_StringBufferSize;

	bool			m_bInsideArray;

	const void* m_pExtraData;

	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;

	void* m_ProxyFn;
	void* m_DataTableProxyFn;	// For RDT_DataTable.

	RecvTable* m_pDataTable;		// For RDT_DataTable.
	int						m_Offset;

	int						m_ElementStride;
	int						m_nElements;

	const char* m_pParentArrayPropName;
};

class ClientClass
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
public:
	ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};
/*class netvars : public singleton <netvars> {
public:
	intptr_t GetOffset(RecvTable* table, const char* tableName, const char* netvarName)
	{
		for (int i = 0; i < table->m_nProps; i++)
		{
			RecvProp prop = table->m_pProps[i];

			if (!_stricmp(prop.m_pVarName, netvarName))
			{
				return prop.m_Offset;
			}

			if (prop.m_pDataTable)
			{
				intptr_t offset = GetOffset(prop.m_pDataTable, tableName, netvarName);

				if (offset)
				{
					return offset + prop.m_Offset;
				}
			}
		}
		return 0;
	}

	intptr_t GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass)
	{
		ClientClass* currNode = clientClass;

		for (auto currNode = clientClass; currNode; currNode = currNode->m_pNext)
		{
			if (!_stricmp(tableName, currNode->m_pRecvTable->m_pNetTableName))
			{
				return GetOffset(currNode->m_pRecvTable, tableName, netvarName);
			}
		}

		return 0;
	}

};
inline  netvars g_netvar;*/