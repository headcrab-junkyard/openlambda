/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2022 BlackPhrase
 *
 * OpenLambda Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenLambda Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenLambda Project. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include <CommonTypes.hpp>

interface INetMsg
{
	///
	virtual void ReadFrom(const IReadBuffer &aBuffer) = 0;
	
	///
	virtual void WriteTo(IWriteBuffer &aBuffer) = 0;
	
	///
	virtual void SetHandler(INetMsgHandler *apHandler) = 0;
	
	///
	//virtual const char *GetName() const = 0;
	
	///
	//virtual int GetID() const = 0; // TODO: GetType?
};

class CBaseNetMsg : public INetMsg
{
public:
	CBaseNetMsg(int anID, const char *asName) : mnID(anID), msName(asName){}
	
	void SetHandler(INetMsgHandler *apHandler) override final {mpHandler = apHandler;}
	
	const char *GetName() const {return msName;}
	int GetID() const {return mnID;}
private:
	INetMsgHandler *mpHandler{nullptr};
	
	const char *msName{nullptr};
	
	int mnID{0};
};

// TODO: IUserMsgHandler?
interface INetMsgHandler
{
	///
	virtual bool Handle(INetClient *cl, INetMsg *net_message) = 0;
};

class CCLC_MoveMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ReadClientMove(cl, &cl->lastcmd); // TODO
		// TODO: qw
		/*
		if(move_issued)
			return false; // someone is trying to cheat...

		move_issued = true;

		//int checksumIndex = net_message->GetReadCount(); // TODO
		byte checksum = (byte)net_message->ReadByte();

		// read loss percentage
		//cl->lossage = net_message->ReadByte(); // TODO

		//net_message->ReadDeltaUsercmd(&nullcmd, &oldest); // TODO
		net_message->ReadDeltaUsercmd(&oldest, &oldcmd);
		net_message->ReadDeltaUsercmd(&oldcmd, &newcmd);
		*/
		
		if(!cl->spawned)
			return false;
		
		/*
		// if the checksum fails, ignore the rest of the packet
		// TODO
		//byte calculatedChecksum = COM_BlockSequenceCRCByte(
			//net_message->data + checksumIndex + 1,
			//net_message->GetReadCount() - checksumIndex - 1,
			//seq_hash);

		if(calculatedChecksum != checksum)
		{
			mpSystem->DevPrintf("Failed command checksum for %s(%d) (%d != %d)\n", 
				cl->GetName(), cl->GetNetChan()->incoming_sequence, checksum, calculatedChecksum);
			return;
		};
		*/

		if(!sv.paused)
		{
			SV_PreRunCmd(cl->edict, &cl->lastcmd, 0); // TODO: get random seed

			if(net_drop < 20)
			{
				while(net_drop > 2)
				{
					SV_RunCmd(&cl->lastcmd);
					net_drop--;
				};
				if(net_drop > 1)
					SV_RunCmd(&oldest);
				if(net_drop > 0)
					SV_RunCmd(&oldcmd);
			};
			SV_RunCmd(&newcmd);

			SV_PostRunCmd();
		};

		cl->lastcmd = newcmd;
		cl->lastcmd.buttons = 0; // avoid multiple fires on lag
		
		return true;
	};
private:
	{
	};
private:
};

class CCLC_DeltaMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ParseDelta(cl, net_message);
		return true;
	};
private:
	void SV_ParseDelta(client_t *cl, INetMsg *net_message)
	{
		cl->delta_sequence = net_message->ReadByte();
	};
};

class CCLC_ResourceListMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ParseResourceList(cl, net_message);
		//SV_NextUpload();
		
		return true;
	};
private:
	void SV_ParseResourceList(client_t *cl, INetMsg *net_message)
	{
		// TODO
		//SV_NextUpload();
	};
};

class CCLC_TeleportMoveMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		// only allowed by spectators
		//if(cl->spectator) // TODO
		{
			vec3_t o;
			
			o[0] = net_message->ReadCoord();
			o[1] = net_message->ReadCoord();
			o[2] = net_message->ReadCoord();
			
			VectorCopy(o, sv_player->v.origin);
			SV_LinkEdict(sv_player, false);
		};
		
		return true;
	};
};

class CCLC_VoiceDataMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ParseVoiceData(cl, net_message, TODO_SomeReadBuffer);
		
		return true;
	};
private:
	void ReadFrom(const IReadBuffer &aBuffer)
	{
		mnPlayerIndex = aBuffer.ReadByte();
		mnSize = aBuffer.ReadWord();
		
		if(mpData)
			Mem_Free(mpData);
		
		mpData = reinterpret_cast<byte*>(Mem_Alloc(mnSize));
		
		for(int i = 0; i < mnSize; ++i)
			mpData[i] = aBuffer.ReadByte();
	};
	
	void WriteTo(IWriteBuffer &aBuffer)
	{
		aBuffer.WriteByte(mnPlayerIndex);
		aBuffer.WriteWord(mnSize);
		
		// TODO: init the mpData
		
		for(int i = 0; i < mnSize; ++i)
			aBuffer.WriteByte(mpData[i]);
	};
	
	void SV_ParseVoiceData(client_t *cl, const IReadBuffer &aBuffer)
	{
		ReadFrom(SomeBuffer);
		
		// Send the data to other clients
		// TODO: should this ignore the client the data came from?
		for(int i = 0; i < svs.clients; ++i)
			SV_SendVoiceData(svs.clients[i], nPlayerIndex, pData, nSize);
		
		// TODO: something else?
		
		Mem_Free(pData);
	};
private:
	byte *mpData{nullptr};
	
	int mnPlayerIndex{0};
	int nSize{0};
};

class CCLC_HLTVMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ParseHLTV(cl, net_message);
		
		return true;
	};
private:
	// TODO: SV_IgnoreHLTV?
	void SV_ParseHLTV(client_t *cl)
	{
		// TODO
	};
};

class CCLC_CVarValueMsgHandler final : public INetMsgHandler
{
public:
	bool Handle(INetClient *cl, INetMsg *net_message) override
	{
		SV_ParseCvarValueResponse(cl, net_message);
		
		return true;
	};
private:
	void SV_ParseCvarValueResponse(client_t *cl)
	{
		const char *sCvarValue = MSG_ReadString();
		
		// TODO: something else?
		
		if(gNewDLLFunctions.pfnCvarValue)
			gNewDLLFunctions.pfnCvarValue(cl->edict, sCvarValue); // TODO: why there is no cvar name here?
	};
};