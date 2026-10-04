VerdanturfTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VerdanturfTown_OnTransition
	.byte 0

VerdanturfTown_OnTransition:
	setflag FLAG_VISITED_VERDANTURF_TOWN
	setvar VAR_CONTEST_HALL_STATE, 0
	end

VerdanturfTown_EventScript_Twin::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_TwinTunnelOpen
	msgbox VerdanturfTown_Text_ManTryingToDigTunnel, MSGBOX_DEFAULT
	applywaitmovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	release
	end

VerdanturfTown_EventScript_TwinTunnelOpen::
	msgbox VerdanturfTown_Text_ManDugTunnelForLove, MSGBOX_DEFAULT
	applywaitmovement LOCALID_VERDANTURF_TWIN, Common_Movement_FaceOriginalDirection
	release
	end

VerdanturfTown_EventScript_Man::
	msgbox VerdanturfTown_Text_AirCleanHere, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Camper::
	msgbox VerdanturfTown_Text_MakeBattleTentDebut, MSGBOX_NPC
	end

VerdanturfTown_EventScript_Boy::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, VerdanturfTown_EventScript_BoyTunnelOpen
	msgbox VerdanturfTown_Text_GuyTryingToBustThroughCave, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_BoyTunnelOpen::
	msgbox VerdanturfTown_Text_EasyToGetToRustboroNow, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_EventScript_TownSign::
	msgbox VerdanturfTown_Text_TownSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_WandasHouseSign::
	msgbox VerdanturfTown_Text_WandasHouse, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_BattleTentSign::
	msgbox VerdanturfTown_Text_BattleTentSign, MSGBOX_SIGN
	end

VerdanturfTown_EventScript_RusturfTunnelSign::
	msgbox VerdanturfTown_Text_RusturfTunnelSign, MSGBOX_SIGN
	end

VerdanturfTown_Text_ManTryingToDigTunnel:
	.string "My papa told me.\p"
	.string "He says this tunnel is full of timid\n"
	.string "POKéMON.\p"
	.string "Loud noises scare them and they make a\n"
	.string "huge racket.\p"
	.string "So they had to stop the big tunnel\n"
	.string "project.\p"
	.string "But there's one man. He's trying to dig\n"
	.string "the tunnel all by himself!$"

VerdanturfTown_Text_ManDugTunnelForLove:
	.string "There was a man who dug a tunnel for\n"
	.string "the lady he loved.\p"
	.string "I don't really get it, but wow!$"

VerdanturfTown_Text_AirCleanHere:
	.string "The way the winds blow, volcanic ash\n"
	.string "never drifts over here.\p"
	.string "The air is clean and delicious.\p"
	.string "Living here should do wonders for even\n"
	.string "the frail and sickly.$"

VerdanturfTown_Text_MakeBattleTentDebut:
	.string "My POKéMON and I have been on a hot\n"
	.string "winning streak.\p"
	.string "So I decided to make my BATTLE TENT\n"
	.string "debut in this town.$"

VerdanturfTown_Text_GuyTryingToBustThroughCave:
	.string "Did you see the cave next to the\n"
	.string "POKéMON MART?\p"
	.string "There's a guy in there smashing\n"
	.string "boulders, trying to bust through to the\l"
	.string "other side.\p"
	.string "If only someone could KAMEHAMEHA\n"
	.string "those rocks out of the way…\p"
	.string "A way through would make it so easy to\n"
	.string "visit RUSTBORO.$"

VerdanturfTown_Text_EasyToGetToRustboroNow:
	.string "The cave next to the POKéMON MART is a\n"
	.string "tunnel to the other side now.\p"
	.string "It's great! Shopping for new CAPSULE\n"
	.string "CORP. products in RUSTBORO is a breeze.$"

VerdanturfTown_Text_TownSign:
	.string "VERDANTURF TOWN\p"
	.string "“Windswept highlands sweet with the\n"
	.string "scent of grass.”$"

VerdanturfTown_Text_WandasHouse:
	.string "WANDA'S HOUSE$"

VerdanturfTown_Text_BattleTentSign:
	.string "BATTLE TENT VERDANTURF SITE\n"
	.string "“Feast Your Eyes on a Fight!”$"

VerdanturfTown_Text_RusturfTunnelSign:
	.string "RUSTURF TUNNEL\n"
	.string "“Linking RUSTBORO and VERDANTURF”\p"
	.string "“The tunnel project has been\n"
	.string "canceled.”$"

