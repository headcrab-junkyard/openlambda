/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2023 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#include "CreateMultiplayerGameBotPage.hpp"

// For bot join team combo box
enum class eBotGUITeamType : int
{
	Random = 0,
	CT,
	T
};

// For bot chatter combo box
enum class eBotGUIChatterType : int
{
	Normal = 0,
	Minimal,
	Radio,
	Off
};

// These must correlate with above enums
static const char *gsJoinTeamArgs[] = {"any", "ct", "t", nullptr};
static const char *gsChatterArgs[] = {"normal", "minimal", "radio", "off", nullptr};

void UpdateValue(KeyValues *apData, const char *asCvarName, int anValue)
{
	apData->SetInt(asCvarName, anValue);
	
	ConVarRef var(asCvarName);
	var.SetValue(anValue);
};

void UpdateValue(KeyValues *apData, const char *asCvarName, const char *asValue)
{
	apData->SetString(asCvarName, asValue);
	
	ConVarRef var(asCvarName);
	var.SetValue(asValue);
};

CCreateMultiplayerGameBotPage::CCreateMultiplayerGameBotPage(vgui::Panel *apParent, const char *asName, KeyValues *apBotKeys) : vgui::PropertyPage(apParent, asName)
{
	mpSavedData = apBotKeys;
	
	mpAllowRoguesCheckBox = new CCvarToggleCheckButton(this, "BotAllowRogueCheck", "", "bot_allow_rogues");
	
	mpAllowPistolsCheckBox = new CCvarToggleCheckButton(this, "BotAllowPistolsCheck", "", "bot_allow_pistols");
	mpAllowShotgunsCheckBox = new CCvarToggleCheckButton(this, "BotAllowShotgunsCheck", "", "bot_allow_shotguns");
	mpAllowSMGsCheckBox = new CCvarToggleCheckButton(this, "BotAllowSubmachineGunsCheck", "", "bot_allow_sub_machine_guns");
	mpAllowRiflesCheckBox = new CCvarToggleCheckButton(this, "BotAllowRiflesCheck", "", "bot_allow_rifles");
	mpAllowSnipersCheckBox = new CCvarToggleCheckButton(this, "BotAllowSnipersCheck", "", "bot_allow_snipers");
	mpAllowMachineGunsCheckBox = new CCvarToggleCheckButton(this, "BotAllowMachineGunsCheck", "", "bot_allow_machine_guns");
	mpAllowGrenadesCheckBox = new CCvarToggleCheckButton(this, "BotAllowGrenadesCheck", "", "bot_allow_grenades");
	
#ifdef OPENLAMBDA_CS_SHIELD_ENABLED
	mpAllowShieldsCheckBox = new CCvarToggleCheckButton(this, "BotAllowShieldCheck", "", "bot_allow_shield");
#endif
	
	mpJoinAfterPlayerCheckBox = new CCvarToggleCheckButton(this, "BotJoinAfterPlayerCheck", "", "bot_join_after_player");
	mpDeferToHumanCheckBox = new CCvarToggleCheckButton(this, "BotDeferToHumanCheck", "", "bot_defer_to_human");
	
	// Set up the team join combo box
	// NOTE: If order of AddItem is changed, update the associated enum
	mpJoinTeamCombo = new ComboBox(this, "BotJoinTeamCombo", 3, false); // TODO: fix magic
	mpJoinTeamCombo->AddItem("#Cstrike_Random", nullptr);
	mpJoinTeamCombo->AddItem("#Cstrike_ScoreBoard_CT", nullptr);
	mpJoinTeamCombo->AddItem("#Cstrike_ScoreBoard_Ter", nullptr);
	
	// Set up the bot chatter combo box
	// NOTE: If order of AddItem is changed, update the associated enum
	mpChatterCombo = new ComboBox(this, "BotChatterCombo", 4, false); // TODO: fix magic
	mpChatterCombo->AddItem("#Cstrike_Bot_Chatter_Normal", nullptr);
	mpChatterCombo->AddItem("#Cstrike_Bot_Chatter_Minimal", nullptr);
	mpChatterCombo->AddItem("#Cstrike_Bot_Chatter_Radio", nullptr);
	mpChatterCombo->AddItem("#Cstrike_Bot_Chatter_Off", nullptr);
	
	// Create text entry fields for bot quota and prefix
	mpPrefixEntry = new TextEntry(this, "BotPrefixEntry");
	
	// Set positions and sizes from resource file
	LoadControlSettings("Resource/CreateMultiplayerGameBotPage.res");
	
	// Get initial values from bot keys
	mpAllowRoguesCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_rogues", true));
	
	mpAllowPistolsCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_pistols", true));
	mpAllowShotgunsCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_shotguns", true));
	mpAllowSMGsCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_sub_machine_guns", true));
	mpAllowRiflesCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_rifles", true));
	mpAllowSnipersCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_snipers", true));
	mpAllowMachineGunsCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_machine_guns", true));
	mpAllowGrenadesCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_grenades", true));
	
#ifdef OPENLAMBDA_CS_SHIELD_ENABLED
	mpAllowShieldsCheckBox->SetSelected(apBotKeys->GetBool("bot_allow_shield", true));
#endif
	
	mpJoinAfterPlayerCheckBox->SetSelected(apBotKeys->GetBool("bot_join_after_player", true));
	mpDeferToHumanCheckBox->SetSelected(apBotKeys->GetBool("bot_defer_to_human", true));
	
	SetJoinTeamCombo(apBotKeys->GetString("bot_join_team", "any"));
	SetChatterCombo(apBotKeys->GetString("bot_chatter", "normal"));
	
	// Set bot prefix
	auto sPrefix{apBotKeys->GetString("bot_prefix")};
	if(sPrefix)
		SetControlString("BotPrefixEntry", sPrefix);
};

CCreateMultiplayerGameBotPage::~CCreateMultiplayerGameBotPage()
{
	// VGUI handles deletion of children automagically through the hierarchy
};

void CCreateMultiplayerGameBotPage::OnResetChanges()
{
};

void CCreateMultiplayerGameBotPage::OnApplyChanges()
{
	UpdateValue(mpSavedData, "bot_allow_rogues", mpAllowRoguesCheckBox->IsSelected());
	
	UpdateValue(mpSavedData, "bot_allow_pistols", mpAllowPistolsCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_shotguns", mpAllowShotgunsCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_sub_machine_guns", mpAllowSMGsCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_rifles", mpAllowRiflesCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_snipers", mpAllowSnipersCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_machine_guns", mpAllowMachineGunsCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_allow_grenades", mpAllowGrenadesCheckBox->IsSelected());
	
#ifdef OPENLAMBDA_CS_SHIELD_ENABLED
	UpdateValue(mpSavedData, "bot_allow_shield", mpAllowShieldsCheckBox->IsSelected());
#endif

	UpdateValue(mpSavedData, "bot_join_after_player", mpJoinAfterPlayerCheckBox->IsSelected());
	UpdateValue(mpSavedData, "bot_defer_to_human", mpDeferToHumanCheckBox->IsSelected());
	
	UpdateValue(mpSavedData, "bot_join_team", gsJoinTeamArgs[mpJoinTeamCombo->GetActiveItem()]);
	
	UpdateValue(mpSavedData, "bot_chatter", gsChatterArgs[mpChatterCombo->GetActiveItem()]);
	
	constexpr auto BUF_LEN{256};
	char sEntryBuffer[BUF_LEN]{};
	mpPrefixEntry->GetText(sEntryBuffer, BUF_LEN);
	UpdateValue(mpSavedData, "bot_prefix", sEntryBuffer);
};

void CCreateMultiplayerGameBotPage::SetJoinTeamCombo(const char *asValue)
{
	if(asValue)
		for(int i = 0; gsJoinTeamArgs[i]; ++i)
			if(!stricmp(asValue, gsJoinTeamArgs[i]))
			{
				mpJoinTeamCombo->ActivateItemByRow(i);
				return;
			};
	else
		mpJoinTeamCombo->ActivateItemByRow(eBotGUITeamType::Random);
};

void CCreateMultiplayerGameBotPage::SetChatterCombo(const char *asValue)
{
	if(asValue)
		for(int i = 0; gsChatterArgs[i]; ++i)
			if(!stricmp(asValue, gsChatterArgs[i]))
			{
				mpChatterCombo->ActivateItemByRow(i);
				return;
			};
	else
		mpChatterCombo->ActivateItemByRow(eBotGUIChatterType::Normal);
};