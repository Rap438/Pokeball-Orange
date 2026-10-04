Route116_TunnelersRestHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route116_TunnelersRestHouse_OnTransition
	.byte 0

Route116_TunnelersRestHouse_OnTransition:
	setflag FLAG_LANDMARK_TUNNELERS_REST_HOUSE
	end

Route116_TunnelersRestHouse_EventScript_Tunneler1::
	msgbox Route116_TunnelersRestHouse_Text_WeHadToStopBoring, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler2::
	msgbox Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf, MSGBOX_NPC
	end

Route116_TunnelersRestHouse_EventScript_Tunneler3::
	lock
	faceplayer
	goto_if_set FLAG_RUSTURF_TUNNEL_OPENED, Route116_TunnelersRestHouse_EventScript_TunnelOpened
	msgbox Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_EventScript_TunnelOpened::
	msgbox Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough, MSGBOX_DEFAULT
	release
	end

Route116_TunnelersRestHouse_Text_WeHadToStopBoring:
	.string "That RUSTURF TUNNEL out there…\p"
	.string "At first we had a huge crew boring\n"
	.string "through the rock with the latest\l"
	.string "machinery. But we had to stop.\p"
	.string "It turns out we were upsetting the wild\n"
	.string "POKéMON in the area.\p"
	.string "So now we've got nothing to do but loll\n"
	.string "around here.$"

Route116_TunnelersRestHouse_Text_ManDiggingHisWayToVerdanturf:
	.string "There's a man digging his way to\n"
	.string "VERDANTURF all by himself. He's\l"
	.string "desperate to get through.\p"
	.string "He says that if he digs little by little\n"
	.string "without machines, he won't disturb the\l"
	.string "POKéMON or harm nature.\p"
	.string "I wonder if he's made it through yet.$"

Route116_TunnelersRestHouse_Text_GetToVerdanturfWithoutTunnel:
	.string "To reach VERDANTURF without this\n"
	.string "TUNNEL, you'd have to cross the sea to\l"
	.string "DEWFORD, sail on to SLATEPORT, then\l"
	.string "travel through MAUVILLE.$"

Route116_TunnelersRestHouse_Text_TunnelHasGoneThrough:
	.string "Did you hear? The TUNNEL to VERDANTURF\n"
	.string "has gone through!\p"
	.string "Sometimes, if you hope hard enough,\n"
	.string "dreams do come true.$"

