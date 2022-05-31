#pragma once
#include <cstddef>
#include <rpcndr.h>
#include <utils\Vector.h>
//#include "../sdk/csgo_struct.h"
#define MAX_SPLITSCREEN_PLAYERS 1
class CBaseHandle;
class c_base_entity;
class IClientEntity;
class c_user_cmd;
class c_base_player;
class IMoveHelper
{
private:
    virtual void unknown() = 0;
public:
    virtual void set_host(c_base_entity* host) = 0;
private:
    virtual void unknown1() = 0;
    virtual void unknown2() = 0;
public:
    virtual bool unknown_func() = 0;
};
class c_move_data {  // NOLINT(hicpp-member-init)
public:
    bool first_run_of_functions : 1;
    bool game_code_moved_player : 1;
    bool no_air_control : 1;

    unsigned long player_handle{};
    int impulse_command{};
    Vector view_angles;
    Vector abs_view_angles;
    int	buttons{};
    int	old_buttons{};
    float forward_move{};
    float side_move{};
    float up_move{};

    float max_speed{};
    float client_max_speed{};

    Vector velocity;
    Vector old_velocity;
    float somefloat{};
    Vector angles;
    Vector old_angles;

    float out_step_height{};
    Vector out_wish_velocity;
    Vector out_jump_velocity;

    Vector constraint_center;
    float constraint_radius{};
    float constraint_width{};
    float constraint_speed_factor{};
    bool constraint_past_radius{};

    Vector abs_origin;
};
class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	ProcessMovement(c_base_entity* pPlayer, c_move_data* pMove) = 0;
    virtual void	Reset(void) = 0;
    virtual void	StartTrackPredictionErrors(c_base_entity* pPlayer) = 0;
    virtual void	FinishTrackPredictionErrors(c_base_entity* pPlayer) = 0;
    virtual void	DiffPrint(char const* fmt, ...) = 0;

    virtual Vector const& GetPlayerMins(bool ducked) const = 0;
    virtual Vector const& GetPlayerMaxs(bool ducked) const = 0;
    virtual Vector const& GetPlayerViewOffset(bool ducked) const = 0;

    virtual bool			IsMovingPlayerStuck(void) const = 0;
    virtual C_BaseEntity* GetMovingPlayer(void) const = 0;
    virtual void			UnblockPusher(c_base_entity* pPlayer, c_base_entity* pPusher) = 0;

    virtual void    SetupMovementBounds(c_move_data* pMove) = 0;
};
class CPrediction
{
    // Construction
public:

    virtual ~CPrediction(void) = 0;//

    virtual void Init(void) = 0;//
    virtual void Shutdown(void) = 0;//

                                    // Implement IPrediction
public:

    virtual void Update
    (
        int startframe, // World update ( un-modded ) most recently received
        bool validframe, // Is frame data valid
        int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
        int outgoing_command // Last command (most recent) sent to server (un-modded)
    );//

    virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet);//
    virtual void PostEntityPacketReceived(void);//5
    virtual void PostNetworkDataReceived(int commands_acknowledged);//

    virtual void OnReceivedUncompressedPacket(void);//

                                                    // The engine needs to be able to access a few predicted values
    virtual void GetViewOrigin(Vector& org);//
    virtual void SetViewOrigin(Vector& org);//
    virtual void GetViewAngles(Vector& ang);//10
    virtual void SetViewAngles(Vector& ang);//

    virtual void GetLocalViewAngles(Vector& ang);//
    virtual void SetLocalViewAngles(Vector& ang);//

    virtual bool InPrediction(void) const;//14
    virtual bool IsFirstTimePredicted(void) const;//

    virtual int GetLastAcknowledgedCommandNumber(void) const;//

#if !defined( NO_ENTITY_PREDICTION )
    virtual int GetIncomingPacketNumber(void) const;//
#endif

    virtual void CheckMovingGround(IClientEntity* player, double frametime);//
    virtual void RunCommand(IClientEntity* player, c_user_cmd* cmd, IMoveHelper* moveHelper);//

    virtual void SetupMove(c_base_entity* player, c_user_cmd* cmd, IMoveHelper* pHelper, c_move_data* move);//20
    virtual void FinishMove(c_base_entity* player, c_user_cmd* cmd, c_move_data* move);//
    virtual void SetIdealPitch(int nSlot, IClientEntity* player, const Vector& origin, const Vector& angles, const Vector& viewheight);//

    virtual void CheckError(int nSlot, IClientEntity* player, int commands_acknowledged);//

public:
    virtual void _Update
    (
        int nSlot,
        bool received_new_world_update,
        bool validframe,            // Is frame data valid
        int incoming_acknowledged,  // Last command acknowledged to have been run by server (un-modded)
        int outgoing_command        // Last command (most recent) sent to server (un-modded)
    );

    // Actually does the prediction work, returns false if an error occurred
    bool PerformPrediction(int nSlot, IClientEntity* localPlayer, bool received_new_world_update, int incoming_acknowledged, int outgoing_command);

    void ShiftIntermediateDataForward(int nSlot, int slots_to_remove, int previous_last_slot);

    void RestoreEntityToPredictedFrame(int nSlot, int predicted_frame);

    int ComputeFirstCommandToExecute(int nSlot, bool received_new_world_update, int incoming_acknowledged, int outgoing_command);

    void DumpEntity(IClientEntity* ent, int commands_acknowledged);

    void ShutdownPredictables(void);

    void ReinitPredictables(void);

    void RemoveStalePredictedEntities(int nSlot, int last_command_packet);

    void RestoreOriginalEntityState(int nSlot);

    void RunSimulation(int current_command, float curtime, c_user_cmd* cmd, IClientEntity* localPlayer);

    void Untouch(int nSlot);

    void StorePredictionResults(int nSlot, int predicted_frame);

    bool ShouldDumpEntity(IClientEntity* ent);

    void SmoothViewOnMovingPlatform(IClientEntity* pPlayer, Vector& offset);

    void ResetSimulationTick();

    void ShowPredictionListEntry(int listRow, int showlist, IClientEntity* ent, int& totalsize, int& totalsize_intermediate);

    void FinishPredictionList(int listRow, int showlist, int totalsize, int totalsize_intermediate);

    void CheckPredictConvar();

#if !defined( NO_ENTITY_PREDICTION )

#endif
};

