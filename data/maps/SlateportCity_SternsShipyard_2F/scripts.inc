SlateportCity_SternsShipyard_2F_MapScripts::
	.byte 0

SlateportCity_SternsShipyard_2F_EventScript_Scientist1::
	msgbox SlateportCity_SternsShipyard_2F_Text_ShipDesignMoreLikeBuilding, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_2F_EventScript_Scientist2::
	msgbox SlateportCity_SternsShipyard_2F_Text_FloatsBecauseBuoyancy, MSGBOX_NPC
	end

SlateportCity_SternsShipyard_2F_Text_ShipDesignMoreLikeBuilding:
	.string "Designing a big ship is more like\n"
	.string "building a skyscraper than assembling a\l"
	.string "vehicle.$"

SlateportCity_SternsShipyard_2F_Text_FloatsBecauseBuoyancy:
	.string "Isn't it strange that a ship made of\n"
	.string "heavy iron floats?\p"
	.string "It floats thanks to a principle called\n"
	.string "buoyancy.$"

