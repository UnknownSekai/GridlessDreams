#pragma once
// Generated from server-of-dreams models/keys.py. Do not edit by hand.
// MessagePack-CSharp [Key] field table driving wire::to_wire / from_array.
namespace wire {
struct FieldSpec { int key; const char* fn; const char* base; bool is_array; const char* kind; bool nullable; };
struct ModelSpec { const char* name; const FieldSpec* fields; int count; };
static const FieldSpec _k_Actor[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"character_master_id","long",false,"prim",false},
  {3,"awakening_phase","int",false,"prim",false},
  {4,"talent_stage","int",false,"prim",false},
  {5,"position","int",false,"prim",false},
  {6,"sense_level","int",false,"prim",false},
  {7,"base_status","LiveStatus",false,"model",true},
  {8,"current_status","LiveStatus",false,"model",true},
  {9,"display_awakening_status","bool",false,"prim",false},
  {10,"secondary_character_base_master_id","long",false,"prim",true},
  {11,"secondary_sense_level","int",false,"prim",false},
  {12,"selection_type","CharacterSelectionTypes",false,"enum",false}
};
static const FieldSpec _k_LiveStatus[] = {
  {0,"concentration","int",false,"prim",false},
  {1,"expression","int",false,"prim",false},
  {2,"vocal","int",false,"prim",false},
  {3,"total_status","int",false,"prim",false}
};
static const FieldSpec _k_Fault[] = {
  {0,"error_code","string",false,"prim",true},
  {1,"message","string",false,"prim",true},
  {2,"stack_trace","string",false,"prim",true}
};
static const FieldSpec _k_DeletedDataObject[] = {
  {0,"type_name","string",false,"prim",true},
  {1,"id_","long",false,"prim",false}
};
static const FieldSpec _k_AbilityVarietyUpPayload[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"variety_to","int",false,"prim",false},
  {2,"item_master_id","long",false,"prim",false}
};
static const FieldSpec _k_AcceptFriendRequest[] = {
  {0,"request_user_id","string",false,"prim",true},
  {1,"received_user_id","string",false,"prim",true},
  {2,"received_user_name","string",false,"prim",true}
};
static const FieldSpec _k_Accessory[] = {
  {0,"id_","long",false,"prim",false},
  {1,"accessory_master_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"locked","bool",false,"prim",false},
  {4,"accessory_effects","long",true,"prim",true},
  {5,"reference_counting","int",false,"prim",false},
  {6,"is_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_AccessoryAutoSell[] = {
  {0,"id_","long",false,"prim",false},
  {1,"auto_sell_rarity","PossessionRarityFlag",false,"enum",false}
};
static const FieldSpec _k_AccessoryAutoSellConvertThing[] = {
  {0,"accessory_master_id","long",false,"prim",false},
  {1,"convert_thing","ReceivedThing",false,"model",true}
};
static const FieldSpec _k_AccessoryEffectMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"effect_master_id","long",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"description","string",false,"prim",true},
  {4,"variety","int",false,"prim",true}
};
static const FieldSpec _k_AccessoryFavoritePayload[] = {
  {0,"accessory_id","long",false,"prim",false},
  {1,"set_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_AccessoryLevelPatternGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"patterns","AccessoryLevelPatternMaster",true,"model",true}
};
static const FieldSpec _k_AccessoryLevelPatternItemMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_AccessoryLevelPatternMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"accessory_level_pattern_group_master_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"required_coin","int",false,"prim",false},
  {4,"items","AccessoryLevelPatternItemMaster",true,"model",true}
};
static const FieldSpec _k_AccessoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"rarity","PossessionRarities",false,"enum",false},
  {4,"accessory_level_pattern_group_id","long",false,"prim",false},
  {5,"fixed_accessory_effects","long",true,"prim",true},
  {6,"random_effect_groups","long",true,"prim",true},
  {7,"pronounce_name","string",false,"prim",true},
  {8,"series","int",false,"prim",false},
  {9,"max_level","int",false,"prim",false}
};
static const FieldSpec _k_AccountConnectPayload[] = {
  {0,"provider","AuthenticationProviders",false,"enum",false},
  {1,"token","string",false,"prim",true}
};
static const FieldSpec _k_AccountDeletionResult[] = {
  {0,"is_success","bool",false,"prim",false},
  {1,"error","AccountDeletionErrorTypes",false,"enum",true}
};
static const FieldSpec _k_AccountRegistResult[] = {
  {0,"token","string",false,"prim",true},
  {1,"error_type","AccountRegisterErrorTypes",false,"enum",false}
};
static const FieldSpec _k_AchivementRateRewardMaster[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false},
  {3,"difficulty","MusicDifficulties",false,"enum",false},
  {4,"achivement_rate","Decimal",false,"prim",true}
};
static const FieldSpec _k_AcquirableThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_AcquirableThingsPayload[] = {
  {0,"things","AcquirableThing",true,"model",true}
};
static const FieldSpec _k_ActorPortalCharacterPayload[] = {
  {0,"character_base_id","long",false,"prim",false},
  {1,"character_id","long",false,"prim",false},
  {2,"is_awakening","bool",false,"prim",false}
};
static const FieldSpec _k_Album[] = {
  {0,"id_","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"publish_page_number","int",false,"prim",false},
  {3,"current_preset_order","int",false,"prim",false}
};
static const FieldSpec _k_AlbumArrangingPayload[] = {
  {0,"publishing","bool",false,"prim",false},
  {1,"page","int",false,"prim",false},
  {2,"items","byte",true,"prim",true},
  {3,"m_album_theme_id","long",false,"prim",true}
};
static const FieldSpec _k_AlbumPage[] = {
  {0,"id_","long",false,"prim",false},
  {1,"page","int",false,"prim",false},
  {2,"edit_type","EditTypes",false,"enum",false},
  {3,"publishing","bool",false,"prim",false},
  {4,"items","byte",true,"prim",true},
  {5,"album_theme_master_id","long",false,"prim",true}
};
static const FieldSpec _k_AlbumPageResult[] = {
  {0,"album_page_id","long",false,"prim",false},
  {1,"album_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"page","int",false,"prim",false},
  {4,"edit_type","EditTypes",false,"enum",false},
  {5,"publishing","bool",false,"prim",false},
  {6,"items","byte",true,"prim",true},
  {7,"album_theme_master_id","long",false,"prim",true}
};
static const FieldSpec _k_AlbumPageSearchResult[] = {
  {0,"album_page_result","AlbumPageResult",false,"model",true},
  {1,"is_success","bool",false,"prim",false}
};
static const FieldSpec _k_AlbumPhotoPayload[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"order","int",false,"prim",false}
};
static const FieldSpec _k_AlbumPreset[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false}
};
static const FieldSpec _k_AlbumTheme[] = {
  {0,"id_","long",false,"prim",false},
  {1,"album_theme_master_id","long",false,"prim",false}
};
static const FieldSpec _k_AnotherNotation[] = {
  {0,"id_","long",false,"prim",false},
  {1,"another_notation_master_id","long",false,"prim",false},
  {2,"clear_lamp","ClearLamps",false,"enum",false},
  {3,"rate_grade","AchievementRateGrades",false,"enum",false},
  {4,"achievement_rate_percent_record","Decimal",false,"prim",true}
};
static const FieldSpec _k_AuditionClear[] = {
  {0,"id_","long",false,"prim",false},
  {1,"audition_master_id","long",false,"prim",false},
  {2,"clear_phase","byte",false,"prim",false},
  {3,"audition_clear_party_id","long",false,"prim",false},
  {4,"user_id","string",false,"prim",true},
  {5,"skip_clear_phase","byte",false,"prim",false}
};
static const FieldSpec _k_AuditionClearParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"difficulty","MusicDifficulties",false,"enum",false},
  {3,"slots","AuditionClearPartySlot",true,"model",true},
  {4,"user_name","string",false,"prim",true},
  {5,"rate_grade","AchievementRateGrades",false,"enum",false},
  {9,"player_rank","string",false,"prim",true},
  {10,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_AuditionClearPartySlot[] = {
  {0,"position","int",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"character_level","int",false,"prim",false},
  {3,"character_talent_stage","int",false,"prim",false},
  {4,"character_awakening_phase","int",false,"prim",false},
  {5,"poster_master_id","long",false,"prim",true},
  {6,"poster_level","int",false,"prim",true},
  {7,"poster_breakthrough_phase","int",false,"prim",true},
  {8,"accessory_master_id","long",false,"prim",true},
  {9,"accessory_level","int",false,"prim",true},
  {10,"star_rank","int",false,"prim",false},
  {11,"talent_stage","int",false,"prim",false},
  {12,"awakening_phase","long",false,"prim",false},
  {13,"current_status","Status",false,"model",true},
  {14,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_AuditionClearedInformationParties[] = {
  {0,"parties","AuditionClearParty",true,"model",true},
  {1,"cleared_phase","byte",false,"prim",false}
};
static const FieldSpec _k_AuditionClearedInformationResult[] = {
  {0,"parties_phases","AuditionClearedInformationParties",true,"model",true}
};
static const FieldSpec _k_AuditionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"recommended_company","Companies",false,"enum",false},
  {3,"can_skip","bool",false,"prim",false},
  {4,"sense_notation_master_id","long",false,"prim",false},
  {5,"max_phase","string",false,"prim",true},
  {6,"display_start_at","DateTime",false,"prim",false},
  {7,"display_end_at","DateTime",false,"prim",false},
  {8,"vocal_version","int",false,"prim",false},
  {9,"audition_group_number","int",false,"prim",false},
  {10,"skip_start_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_AuditionPhaseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"auditionaster_id","long",false,"prim",false},
  {2,"phase","byte",false,"prim",false},
  {3,"recommended_player_rank","int",false,"prim",false},
  {4,"clear_score","long",false,"prim",false},
  {5,"star_act_count","int",false,"prim",true},
  {6,"audition_reward_package_master_id","long",false,"prim",false}
};
static const FieldSpec _k_AuditionRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","AuditionRewardThing",true,"model",true}
};
static const FieldSpec _k_AuditionRewardThing[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_AuthenticatePayload[] = {
  {0,"login_token","string",false,"prim",true},
  {1,"game_version","GameVersions",false,"enum",false},
  {2,"apk_hash","string",false,"prim",true},
  {3,"apk_application_signature","string",false,"prim",true},
  {4,"application_version","string",false,"prim",true}
};
static const FieldSpec _k_AuthenticateResult[] = {
  {0,"token","string",false,"prim",true},
  {1,"ban_level","BanLevels",false,"enum",false},
  {2,"warned_until","DateTime",false,"prim",true}
};
static const FieldSpec _k_BannerPayload[] = {
  {0,"circle_hashed_id","string",false,"prim",true},
  {1,"poster_master_id","long",false,"prim",false},
  {2,"x1","float",false,"prim",false},
  {3,"y1","float",false,"prim",false},
  {4,"x2","float",false,"prim",false},
  {5,"y2","float",false,"prim",false},
  {6,"rotation_angle","int",false,"prim",false},
  {7,"display_poster_string","bool",false,"prim",false},
  {8,"banner_ratio","float",false,"prim",false}
};
static const FieldSpec _k_BaseScoreBlock[] = {
  {0,"hash","long",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"life","int",false,"prim",false},
  {3,"note_id","long",false,"prim",false},
  {4,"timing_type","TimingTypes",false,"enum",false},
  {5,"combo","int",false,"prim",false}
};
static const FieldSpec _k_Behaviour[] = {
};
static const FieldSpec _k_BlockListResult[] = {
  {0,"results","FriendResult",true,"model",true}
};
static const FieldSpec _k_Bomb[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bomb_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_BombMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false}
};
static const FieldSpec _k_BonusLive[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bonus_live_master_id","long",false,"prim",false},
  {2,"cleared_stage_order","int",false,"prim",false},
  {3,"read_tips","bool",false,"prim",false},
  {4,"daily_clear_times","int",false,"prim",false}
};
static const FieldSpec _k_BonusLiveResult[] = {
  {0,"rewards","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_BonusLiveStage[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bonus_live_master_stage_id","long",false,"prim",false},
  {2,"clear_times","int",false,"prim",false}
};
static const FieldSpec _k_BooleanResult[] = {
  {0,"is_success","bool",false,"prim",false}
};
static const FieldSpec _k_BranchMaster[] = {
  {0,"order","int",false,"prim",false},
  {1,"branch_effects","EffectOrderMaster",true,"model",true},
  {2,"judge_type1","BranchJudgeTypes",false,"enum",true},
  {3,"parameter1","long",false,"prim",true},
  {4,"judge_type2","BranchJudgeTypes",false,"enum",true},
  {5,"parameter2","long",false,"prim",true},
  {6,"id_","long",false,"prim",false}
};
static const FieldSpec _k_BuffItemStatus[] = {
  {0,"id_","long",false,"prim",false},
  {1,"effect_type","CampaignEffectTypes",false,"enum",false},
  {2,"buff_item_master_id","long",false,"prim",false},
  {3,"valid_until","DateTime",false,"prim",false}
};
static const FieldSpec _k_BulkLevelUpPayload[] = {
  {0,"character_id","long",false,"prim",false},
  {1,"order","int",false,"prim",false}
};
static const FieldSpec _k_BulkReceivePayload[] = {
  {0,"inbox_ids","long",true,"prim",true}
};
static const FieldSpec _k_CalculateLessonTimeEventPayload[] = {
  {0,"music_master_id","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CalculateTimeEventPayload[] = {
  {0,"music_master_id","long",false,"prim",false},
  {1,"sense_notation_master_id","long",false,"prim",true},
  {2,"party_id","long",false,"prim",false},
  {3,"vocal_version","int",false,"prim",false},
  {4,"is_story_event_challenge","bool",false,"prim",false}
};
static const FieldSpec _k_CampaignMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"title","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"icon_image_path","string",false,"prim",true},
  {4,"order","int",false,"prim",false},
  {5,"start_date","DateTime",false,"prim",false},
  {6,"end_date","DateTime",false,"prim",false},
  {7,"comeback_campaign_master_id","long",false,"prim",true},
  {8,"campaign_effect_type","CampaignEffectTypes",false,"enum",false},
  {9,"campaign_effect_value","int",false,"prim",false}
};
static const FieldSpec _k_ChangeNamePayload[] = {
  {0,"name","string",false,"prim",true}
};
static const FieldSpec _k_ChangePhotoAbilityPayload[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"photo_effect_type_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_Character[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"current_experience","int",false,"prim",false},
  {4,"talent_stage","int",false,"prim",false},
  {5,"awakening_phase","int",false,"prim",false},
  {6,"character_base_id","long",false,"prim",false},
  {7,"sense_level","int",false,"prim",false},
  {8,"read_episode_order","CharacterEpisodeOrder",false,"enum",false},
  {9,"released_episode_order","CharacterEpisodeOrder",false,"enum",false},
  {10,"display_awakening_status","bool",false,"prim",false},
  {11,"secondary_character_base_id","long",false,"prim",true},
  {12,"secondary_sense_level","int",false,"prim",false},
  {13,"selection_type","CharacterSelectionTypes",false,"enum",false},
  {14,"is_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_CharacterAwakeningItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {3,"awakening_phase","int",false,"prim",false},
  {4,"item_master_id","long",false,"prim",false},
  {5,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_CharacterBase[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"star_rank","int",false,"prim",false},
  {3,"total_star_point","int",false,"prim",false},
  {4,"costume_master_id","long",false,"prim",true},
  {5,"key_mission_level","int",false,"prim",false},
  {6,"portal_character_id","long",false,"prim",false},
  {7,"portal_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_CharacterBaseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"school","string",false,"prim",true},
  {4,"grade","int",false,"prim",true},
  {5,"birth_month","int",false,"prim",true},
  {6,"birth_day","int",false,"prim",true},
  {7,"height","int",false,"prim",false},
  {8,"hobby","string",false,"prim",true},
  {9,"company_master_id","long",false,"prim",false},
  {10,"name_romanization","string",false,"prim",true},
  {11,"sense_name","string",false,"prim",true},
  {12,"sense_effect","string",false,"prim",true},
  {13,"character_voice","string",false,"prim",true},
  {14,"profile_image_asset_id","string",false,"prim",true},
  {15,"age","int",false,"prim",true},
  {16,"family_name_romanization","string",false,"prim",true},
  {17,"first_name_romanization","string",false,"prim",true},
  {18,"pronounce_family_name","string",false,"prim",true},
  {19,"pronounce_first_name","string",false,"prim",true},
  {20,"family_name","string",false,"prim",true},
  {21,"first_name","string",false,"prim",true},
  {22,"evo_sense_name","string",false,"prim",true},
  {23,"evo_sense_effect","string",false,"prim",true},
  {24,"default_costume_master_id","long",false,"prim",false},
  {25,"character_base_type","CharacterBaseTypes",false,"enum",false}
};
static const FieldSpec _k_CharacterBaseStarPointResult[] = {
  {0,"character_base_master_id","long",false,"prim",false},
  {1,"star_point_result","StarPointResult",false,"model",true},
  {2,"link_character_received_reward","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_CharacterBloomBonusGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bloom_bonuses","CharacterBloomBonusMaster",true,"model",true},
  {2,"bloom_rewards","CharacterBloomRewardMaster",true,"model",true}
};
static const FieldSpec _k_CharacterBloomBonusMaster[] = {
  {1,"bloom_bonus_type","BloomBonusTypes",false,"enum",false},
  {2,"description","string",false,"prim",true},
  {3,"phase","int",false,"prim",false},
  {5,"effect_master_id","long",false,"prim",false},
  {6,"icon_path","string",false,"prim",true}
};
static const FieldSpec _k_CharacterBloomItemMaster[] = {
  {0,"rarity","CharacterRarities",false,"enum",false},
  {1,"current_stage","int",false,"prim",false},
  {2,"required_piece_amount","int",false,"prim",false},
  {3,"talent_bloom_item_type","TalentBloomItemTypes",false,"enum",false},
  {4,"generic_bloom_item_master_id","long",false,"prim",true},
  {5,"required_item_master_id","long",false,"prim",true},
  {6,"required_item_amount","int",false,"prim",true}
};
static const FieldSpec _k_CharacterBloomRewardMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false},
  {4,"phase","int",false,"prim",false}
};
static const FieldSpec _k_CharacterCategoryMaster[] = {
  {0,"category_master_id","long",false,"prim",false},
  {1,"is_awaken","bool",false,"prim",false}
};
static const FieldSpec _k_CharacterExperienceItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"acquirable_experience","long",false,"prim",false},
  {3,"acquirable_experience_bonus","float",false,"prim",false}
};
static const FieldSpec _k_CharacterFavoritePayload[] = {
  {0,"character_id","long",false,"prim",false},
  {1,"set_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_CharacterLesson[] = {
  {0,"character_base_master_id","long",false,"prim",false},
  {1,"set_characters","CharacterLessonSlot",true,"model",true},
  {2,"best_score","long",false,"prim",false},
  {3,"leader_position","int",false,"prim",false},
  {4,"reward_received_high_score","long",false,"prim",false}
};
static const FieldSpec _k_CharacterLessonSlot[] = {
  {0,"position","int",false,"prim",false},
  {1,"set_character_id","long",false,"prim",true}
};
static const FieldSpec _k_CharacterLevelMaster[] = {
  {0,"level","int",false,"prim",false},
  {1,"experience_to_level_up","long",false,"prim",false},
  {2,"character_status_level","int",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_CharacterLineupResult[] = {
  {0,"normal_probabilities","CharacterRarityProbability",true,"model",true},
  {1,"fixed_probabilities","CharacterRarityProbability",true,"model",true},
  {2,"normal_lineup_items","GachaLineupItemProbability",true,"model",true},
  {3,"fixed_lineup_items","GachaLineupItemProbability",true,"model",true}
};
static const FieldSpec _k_CharacterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"description","string",false,"prim",true},
  {4,"asset_id","string",false,"prim",true},
  {5,"rarity","CharacterRarities",false,"enum",false},
  {6,"attribute","Attributes",false,"enum",false},
  {7,"min_level_status","Status",false,"model",true},
  {9,"star_act_master_id","long",false,"prim",false},
  {10,"awaken_star_act_master_id","long",false,"prim",true},
  {11,"sense_master_id","long",false,"prim",false},
  {12,"forbid_generic_item_bloom","bool",false,"prim",false},
  {13,"bloom_bonus_group_master_id","long",false,"prim",false},
  {14,"sense_enhance_item_group_master_id","long",false,"prim",false},
  {15,"first_episode_release_item_group_id","long",false,"prim",false},
  {16,"second_episode_release_item_group_id","long",false,"prim",false},
  {17,"character_awakening_item_group_master_id","long",false,"prim",true},
  {18,"display_start_at","DateTime",false,"prim",false},
  {19,"display_end_at","DateTime",false,"prim",false},
  {20,"unlock_text","string",false,"prim",true},
  {21,"categories","CharacterCategoryMaster",true,"model",true},
  {22,"leader_sense_master_id","long",false,"prim",true},
  {23,"max_talent_stage","int",false,"prim",false},
  {24,"max_talent_stage_release_date","DateTime",false,"prim",true},
  {25,"secondary_character_base_master_id","long",false,"prim",true},
  {26,"secondary_sense_master_id","long",false,"prim",true},
  {27,"secondary_attribute","Attributes",false,"enum",true}
};
static const FieldSpec _k_CharacterMission[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"character_mission_master_id","long",false,"prim",false},
  {3,"current_stage_master_id","long",false,"prim",false},
  {4,"current_count","long",false,"prim",false},
  {5,"cleared_stage_order","int",false,"prim",false},
  {6,"reward_received_stage_order","int",false,"prim",false},
  {7,"completed_level","int",false,"prim",false}
};
static const FieldSpec _k_CharacterMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"title","string",false,"prim",true},
  {2,"jump_type","JumpTypes",false,"enum",true},
  {3,"jump_value","long",false,"prim",true},
  {4,"stages","CharacterMissionStageMaster",true,"model",true}
};
static const FieldSpec _k_CharacterMissionStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_mission_category_level_master_id","long",false,"prim",false},
  {2,"character_mission_master_id","long",false,"prim",false},
  {3,"exclusion_no_sense_character","bool",false,"prim",false},
  {4,"order","int",false,"prim",false},
  {5,"stage_order","int",false,"prim",false},
  {6,"goal_count","long",false,"prim",false}
};
static const FieldSpec _k_CharacterPieceMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"dugong_required_amount","int",false,"prim",false},
  {3,"talent_bloom_item_type","TalentBloomItemTypes",false,"enum",false}
};
static const FieldSpec _k_CharacterPointEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_point_event_master_id","long",false,"prim",false},
  {2,"character_base_master_id","long",false,"prim",true},
  {3,"total_acquired_point","int",false,"prim",false},
  {4,"last_rank","int",false,"prim",false},
  {5,"read_tips","bool",false,"prim",false}
};
static const FieldSpec _k_CharacterPointEventInformationResult[] = {
  {0,"current_total_point","long",false,"prim",false},
  {1,"overall_rank","int",false,"prim",true},
  {2,"character_rank","int",false,"prim",true}
};
static const FieldSpec _k_CharacterPointEventRankingResult[] = {
  {0,"raw_ranking","RawRankingWithLongPoint",true,"model",true},
  {1,"user_profiles","UserProfile",true,"model",true}
};
static const FieldSpec _k_CharacterRank[] = {
  {0,"m_character_base_id","long",false,"prim",false},
  {1,"rank","int",false,"prim",false}
};
static const FieldSpec _k_CharacterRarityProbability[] = {
  {0,"rarity","CharacterRarities",false,"enum",false},
  {1,"probability","double",false,"prim",false}
};
static const FieldSpec _k_CharacterSenseEnhanceItemGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"items","CharacterSenseEnhanceItemMaster",true,"model",true}
};
static const FieldSpec _k_CharacterSenseEnhanceItemMaster[] = {
  {0,"current_level","int",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_CharacterStarRankMaster[] = {
  {0,"rank","int",false,"prim",false},
  {1,"next_rank_point","long",false,"prim",false},
  {2,"required_lesson_score","long",false,"prim",false},
  {3,"status_bonus","float",false,"prim",false}
};
static const FieldSpec _k_CharacterStarRankRewardGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","CharacterStarRankRewardMaster",true,"model",true}
};
static const FieldSpec _k_CharacterStarRankRewardMaster[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_ChatworkSendMessagePayload[] = {
  {0,"message","string",false,"prim",true}
};
static const FieldSpec _k_CircleAuthorityChangePayload[] = {
  {0,"circle_hashed_id","string",false,"prim",true},
  {1,"user_id","string",false,"prim",true},
  {2,"authority","CircleAuthorities",false,"enum",false}
};
static const FieldSpec _k_CircleAuthorityResult[] = {
  {0,"result_status","CircleAuthorityResultStatus",false,"enum",false}
};
static const FieldSpec _k_CircleBanner[] = {
  {0,"poster_master_id","long",false,"prim",false},
  {1,"x1","float",false,"prim",false},
  {2,"y1","float",false,"prim",false},
  {3,"x2","float",false,"prim",false},
  {4,"y2","float",false,"prim",false},
  {5,"rotation_angle","int",false,"prim",false},
  {6,"display_poster_string","bool",false,"prim",false},
  {7,"banner_ratio","float",false,"prim",false}
};
static const FieldSpec _k_CircleEventCircleMissionProgress[] = {
  {0,"circle_event_mission_master_id","long",false,"prim",false},
  {1,"current_count","long",false,"prim",false}
};
static const FieldSpec _k_CircleEventInformationResult[] = {
  {0,"circle_point","long",false,"prim",true},
  {1,"user_point","long",false,"prim",false},
  {2,"last_received_circle_point","long",false,"prim",false},
  {3,"mission_refresh_count","int",false,"prim",false},
  {4,"progresses","CircleEventCircleMissionProgress",true,"model",true}
};
static const FieldSpec _k_CircleEventMission[] = {
  {0,"circle_event_mission_master_id","long",false,"prim",false},
  {1,"current_count","long",false,"prim",false},
  {2,"is_active","bool",false,"prim",false}
};
static const FieldSpec _k_CircleEventRanking[] = {
  {0,"circle_raw_rankings","CircleRawRanking",true,"model",true},
  {1,"circle_profiles","CircleProfile",true,"model",true}
};
static const FieldSpec _k_CircleInformationResult[] = {
  {0,"u_circle_hashed_id","string",false,"prim",true},
  {1,"name","string",false,"prim",true},
  {2,"comment","string",false,"prim",true},
  {3,"play_time_start_type","PlayTimeTypes",false,"enum",false},
  {4,"play_time_end_type","PlayTimeTypes",false,"enum",false},
  {5,"entry_type","EntryTypes",false,"enum",false},
  {6,"member_count","int",false,"prim",false},
  {7,"invite_id","long",false,"prim",false},
  {9,"result_status","CircleResultStatus",false,"enum",false},
  {10,"circle_banner","CircleBanner",false,"model",true},
  {11,"main_character_master_id","long",false,"prim",false},
  {12,"display_awakening_status","bool",false,"prim",false},
  {13,"company_master_id","long",false,"prim",true},
  {14,"character_base_master_id","long",false,"prim",true},
  {15,"icon_fame_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CircleInviteResult[] = {
  {0,"result_status","CircleResultStatus",false,"enum",false},
  {1,"invite_id","long",false,"prim",true}
};
static const FieldSpec _k_CircleMemberInfoParameters[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"user_name","string",false,"prim",true},
  {2,"player_rank","int",false,"prim",false},
  {3,"last_login_at","DateTime",false,"prim",false},
  {4,"authority","CircleAuthorities",false,"enum",true},
  {5,"main_u_character","MainCharacter",false,"model",true},
  {6,"invite_id","long",false,"prim",true},
  {7,"request_id","long",false,"prim",true},
  {8,"trophy_master_id1","long",false,"prim",false},
  {9,"trophy_master_id2","long",false,"prim",false},
  {10,"trophy_master_id3","long",false,"prim",false},
  {11,"main_character_master_id","long",false,"prim",false},
  {12,"display_awakening_status","bool",false,"prim",false},
  {13,"player_rate","Decimal",false,"prim",true},
  {14,"is_public_player_rate","bool",false,"prim",false},
  {15,"league_class","LeagueClassTypes",false,"enum",false},
  {16,"icon_fame_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CircleMemberInfoResult[] = {
  {0,"parameters","CircleMemberInfoParameters",true,"model",true},
  {1,"result_status","CircleResultStatus",false,"enum",false},
  {2,"circle_banner","CircleBanner",false,"model",true}
};
static const FieldSpec _k_CircleMemberParameters[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"user_name","string",false,"prim",true},
  {2,"player_rank","int",false,"prim",false},
  {3,"last_login_at","DateTime",false,"prim",false},
  {4,"authority","CircleAuthorities",false,"enum",true},
  {5,"main_u_character","MainCharacter",false,"model",true},
  {6,"invite_id","long",false,"prim",true},
  {7,"request_id","long",false,"prim",true},
  {8,"trophy_master_id1","long",false,"prim",false},
  {9,"trophy_master_id2","long",false,"prim",false},
  {10,"trophy_master_id3","long",false,"prim",false},
  {11,"main_character_master_id","long",false,"prim",false},
  {12,"display_awakening_status","bool",false,"prim",false},
  {16,"icon_fame_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CirclePayload[] = {
  {0,"hashed_id","string",false,"prim",true},
  {1,"name","string",false,"prim",true},
  {2,"comment","string",false,"prim",true},
  {3,"play_time_start_type","PlayTimeTypes",false,"enum",false},
  {4,"play_time_end_type","PlayTimeTypes",false,"enum",false},
  {5,"entry_type","EntryTypes",false,"enum",false},
  {6,"member_type","SearchCircleMemberConditionTypes",false,"enum",false},
  {7,"company_master_id","long",false,"prim",true},
  {8,"character_base_master_id","long",false,"prim",true}
};
static const FieldSpec _k_CircleProfile[] = {
  {0,"circle_id","string",false,"prim",true},
  {1,"name","string",false,"prim",true},
  {2,"member_count","int",false,"prim",false},
  {3,"main_character_master_id","long",false,"prim",false},
  {4,"display_awakening_status","bool",false,"prim",false},
  {5,"icon_frame_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CircleRawRanking[] = {
  {0,"rank","int",false,"prim",false},
  {1,"point","int",false,"prim",false},
  {2,"circle_id","string",false,"prim",true}
};
static const FieldSpec _k_CircleResult[] = {
  {0,"result_status","CircleResultStatus",false,"enum",false}
};
static const FieldSpec _k_CircleSearchIdResult[] = {
  {0,"parameters","CircleMemberParameters",false,"model",true},
  {1,"result_status","CircleResultStatus",false,"enum",false}
};
static const FieldSpec _k_CircleSearchResult[] = {
  {0,"parameters","CircleMemberParameters",true,"model",true},
  {1,"result_status","CircleResultStatus",false,"enum",false},
  {2,"circle_banner","CircleBanner",false,"model",true}
};
static const FieldSpec _k_ComebackCampaign[] = {
  {0,"comeback_campaign_master_id","long",false,"prim",false},
  {1,"activated_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_Comic[] = {
  {0,"comic_episode_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CompanyMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"companies","Companies",false,"enum",false},
  {3,"description","string",false,"prim",true},
  {4,"is_other","bool",false,"prim",false}
};
static const FieldSpec _k_Component[] = {
};
static const FieldSpec _k_ConcertResult[] = {
  {0,"rewards","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_ConcertStage[] = {
  {0,"concert_stage_master_id","long",false,"prim",false}
};
static const FieldSpec _k_ConcoursDetailInformationResult[] = {
  {0,"concours_detail_master_id","long",false,"prim",false},
  {1,"rank","int",false,"prim",true}
};
static const FieldSpec _k_ConcoursInfomationResult[] = {
  {0,"details","ConcoursDetailInformationResult",true,"model",true},
  {1,"current_point","int",false,"prim",false}
};
static const FieldSpec _k_ConnectWithAccount[] = {
  {0,"id_","long",false,"prim",false},
  {1,"provider","AuthenticationProviders",false,"enum",false}
};
static const FieldSpec _k_ConnectWithPassword[] = {
  {0,"id_","long",false,"prim",false}
};
static const FieldSpec _k_ConvertedThingResult[] = {
  {0,"exchange_shop_master_id","long",false,"prim",false},
  {1,"original_quantity","int",false,"prim",false},
  {2,"received_thing","ReceivedThing",false,"model",true}
};
static const FieldSpec _k_Costume[] = {
  {0,"id_","long",false,"prim",false},
  {1,"costume_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CostumeFavoritePayload[] = {
  {0,"costume_master_id","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"set_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_CostumeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"is_default","bool",false,"prim",false},
  {4,"costume_group_master_id","long",false,"prim",false},
  {5,"description","string",false,"prim",true}
};
static const FieldSpec _k_CostumeWearableCharacterGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_CourseRankingResult[] = {
  {0,"rank","int",false,"prim",false},
  {1,"user_id","long",false,"prim",false},
  {2,"user_profile","UserProfile",false,"model",true},
  {3,"course_result","CourseResult",false,"model",true},
  {4,"note_result","NoteResult",false,"model",true}
};
static const FieldSpec _k_CourseResult[] = {
  {0,"total_achievement_rate_percent_record","Decimal",false,"prim",true},
  {1,"best_record_challenge_count","int",false,"prim",false},
  {2,"best_record_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_CreateCircleResult[] = {
  {0,"result_status","CircleResultStatus",false,"enum",false},
  {1,"circle_hashed_id","string",false,"prim",true}
};
static const FieldSpec _k_CreateMultiRoomPayload[] = {
  {0,"room_name","string",false,"prim",true},
  {1,"password","string",false,"prim",true},
  {2,"can_join_max_member","int",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",false},
  {4,"end_date","DateTime",false,"prim",false},
  {5,"play_mode","MultiRoomPlayModes",false,"enum",false},
  {6,"live_master_id1","long",false,"prim",false},
  {7,"live_master_id2","long",false,"prim",true},
  {8,"live_master_id3","long",false,"prim",true}
};
static const FieldSpec _k_Currency[] = {
  {0,"id_","long",false,"prim",false},
  {1,"coin","int",false,"prim",false},
  {2,"free_jewel","int",false,"prim",false},
  {3,"paid_jewel","int",false,"prim",false}
};
static const FieldSpec _k_DailyLesson[] = {
  {0,"times_left","int",false,"prim",false}
};
static const FieldSpec _k_DailyLimit[] = {
  {0,"id_","long",false,"prim",false},
  {1,"auto_play_times","int",false,"prim",false},
  {2,"daily_lesson_times","int",false,"prim",false},
  {3,"last_refreshed_at","DateTime",false,"prim",true},
  {4,"music_course_free_challenge_times","int",false,"prim",false}
};
static const FieldSpec _k_DebugAlbumSimpleArrangingPayload[] = {
  {0,"publishing","bool",false,"prim",false},
  {1,"page","int",false,"prim",false},
  {2,"photos","AlbumPhotoPayload",true,"model",true},
  {3,"m_album_theme_id","long",false,"prim",true}
};
static const FieldSpec _k_DebugEditLeagueBasicPayload[] = {
  {0,"current_class_type","LeagueClassTypes",false,"enum",false},
  {1,"best_class_type","LeagueClassTypes",false,"enum",false}
};
static const FieldSpec _k_DebugLinkageCodeResult[] = {
  {0,"password","string",false,"prim",true},
  {1,"take_over_code_result","TakeOverCodeResult",false,"model",true}
};
static const FieldSpec _k_DebugModifyCharacterEnhanceInformationPayload[] = {
  {0,"m_character_id","long",false,"prim",false},
  {1,"actor_level","int",false,"prim",false},
  {2,"is_awaken","bool",false,"prim",false},
  {3,"sense_level","int",false,"prim",false},
  {4,"talent_bloom_stage","int",false,"prim",false}
};
static const FieldSpec _k_DebugModifyPosterEnhanceInformationPayload[] = {
  {0,"m_poster_id","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"release_phase","int",false,"prim",false}
};
static const FieldSpec _k_DebugPrepareLeagueGroupUserPayload[] = {
  {0,"league_master_id","long",false,"prim",false},
  {1,"class_type","LeagueClassTypes",false,"enum",false},
  {2,"user_ids","long",true,"prim",true}
};
static const FieldSpec _k_DebugUserIdResult[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"hashed_user_id","string",false,"prim",true}
};
static const FieldSpec _k_Decimal[] = {
};
static const FieldSpec _k_Decoration[] = {
  {0,"id_","long",false,"prim",false},
  {1,"decoration_master_id","long",false,"prim",false}
};
static const FieldSpec _k_Dictionary[] = {
};
static const FieldSpec _k_DonateSupportCompanyResult[] = {
  {0,"my_circle_information","MyCircleInformationResult",false,"model",true},
  {1,"result_status","CircleDonateSupportCompanyResult",false,"enum",false}
};
static const FieldSpec _k_DonateSupportLevelLimitDetail[] = {
  {0,"order","int",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_DonateSupportLevelLimitPayload[] = {
  {0,"company","Companies",false,"enum",false},
  {1,"next_level_limit","int",false,"prim",false},
  {2,"coin_quantity","int",false,"prim",false},
  {3,"details","DonateSupportLevelLimitDetail",true,"model",true}
};
static const FieldSpec _k_DugongRun[] = {
  {0,"id_","long",false,"prim",false},
  {1,"cleared_course_ids","long",true,"prim",true},
  {2,"no_mistake_course_ids","long",true,"prim",true},
  {3,"dugong_run_course_group_id","long",false,"prim",false}
};
static const FieldSpec _k_EditBookmarkPayload[] = {
  {0,"music_master_id","long",false,"prim",false},
  {1,"bookmark_flag","MusicBookmarkFlags",false,"enum",false}
};
static const FieldSpec _k_EditPartyPayload[] = {
  {0,"party_slots","EditPartySlotPayload",true,"model",true},
  {1,"sense_notation_master_id","long",false,"prim",true},
  {2,"music_master_id","long",false,"prim",true},
  {3,"vocal_version","int",false,"prim",true},
  {4,"is_high_score_challenge","bool",false,"prim",false},
  {5,"leader_position","int",false,"prim",true}
};
static const FieldSpec _k_EditPartySlotPayload[] = {
  {0,"u_party_slot_id","long",false,"prim",false},
  {1,"u_character_id","long",false,"prim",false},
  {2,"u_accessory_id","long",false,"prim",true},
  {3,"u_poster_id","long",false,"prim",true},
  {4,"bonus_ability_enable_flags","BonusAbilityEnableFlags",false,"enum",false}
};
static const FieldSpec _k_EditPositionPayload[] = {
  {0,"slots","EditPositionSlotPayload",true,"model",true}
};
static const FieldSpec _k_EditPositionSlotPayload[] = {
  {0,"u_party_slot_id","long",false,"prim",false},
  {1,"position","int",false,"prim",false}
};
static const FieldSpec _k_EditTrialPartyEventStagePartyPayload[] = {
  {0,"trial_party_event_stage_master_id","long",false,"prim",false},
  {1,"slots","EditTrialPartyEventStagePartyPayloadSlot",true,"model",true},
  {2,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_EditTrialPartyEventStagePartyPayloadSlot[] = {
  {0,"position","int",false,"prim",false},
  {1,"trial_party_character_master_id","long",false,"prim",false},
  {2,"trial_party_poster_master_id","long",false,"prim",true},
  {3,"trial_party_accessory_master_id","long",false,"prim",true}
};
static const FieldSpec _k_EditUserProfilePayload[] = {
  {0,"name","string",false,"prim",true},
  {1,"introduction","string",false,"prim",true},
  {2,"main_u_character_id","long",false,"prim",false},
  {3,"m_nameplate_id","long",false,"prim",true},
  {4,"m_name_color_id","long",false,"prim",false},
  {5,"m_trophy_id1","long",false,"prim",true},
  {6,"m_trophy_id2","long",false,"prim",true},
  {7,"m_trophy_id3","long",false,"prim",true},
  {8,"is_public_player_rate","bool",false,"prim",false},
  {9,"display_awakening_status","bool",false,"prim",false},
  {10,"main_character_master_id","long",false,"prim",false},
  {11,"name_base_color_masterid","long",false,"prim",false},
  {12,"icon_frame_master_id","long",false,"prim",false},
  {13,"home_skin_master_id","long",false,"prim",false}
};
static const FieldSpec _k_EexternalPaymentResult[] = {
  {0,"received_jewel_shop_item_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_Effect[] = {
  {0,"order","int",false,"prim",false},
  {1,"master_id","long",false,"prim",false},
  {2,"effect_types","EffectTypes",false,"enum",false},
  {3,"triggers","EffectTrigger",true,"model",true},
  {4,"targets","EffectTargetValue",true,"model",true},
  {5,"duration","int",false,"prim",false}
};
static const FieldSpec _k_EffectBranch[] = {
  {0,"condition_value","long",false,"prim",true},
  {1,"judge_types","BranchJudgeTypes",false,"enum",true},
  {2,"sense_effects","Effect",true,"model",true}
};
static const FieldSpec _k_EffectConditionMaster[] = {
  {0,"condition","EffectConditions",false,"enum",false},
  {1,"value","long",false,"prim",true}
};
static const FieldSpec _k_EffectDetailMaster[] = {
  {0,"level","int",false,"prim",false},
  {1,"value","float",false,"prim",false}
};
static const FieldSpec _k_EffectDurationGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"durations","EffectDurationMaster",true,"model",true}
};
static const FieldSpec _k_EffectDurationMaster[] = {
  {0,"level","int",false,"prim",false},
  {1,"duration_seconds","float",false,"prim",false}
};
static const FieldSpec _k_EffectMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"type","EffectTypes",false,"enum",false},
  {2,"range","EffectTargetRanges",false,"enum",false},
  {3,"calculation_type","CalculationTypes",false,"enum",false},
  {4,"details","EffectDetailMaster",true,"model",true},
  {5,"conditions","EffectConditionMaster",true,"model",true},
  {6,"duration_second","int",false,"prim",false},
  {7,"triggers","EffectTriggerMaster",true,"model",true},
  {8,"fire_timing_type","FireTimingTypes",false,"enum",false}
};
static const FieldSpec _k_EffectOrderMaster[] = {
  {0,"order","int",false,"prim",false},
  {1,"effect_master_id","long",false,"prim",false}
};
static const FieldSpec _k_EffectTargetValue[] = {
  {0,"target_actor_id","long",false,"prim",true},
  {1,"value","double",false,"prim",false}
};
static const FieldSpec _k_EffectTrigger[] = {
  {0,"type","TriggerTypes",false,"enum",false},
  {1,"value","long",false,"prim",false}
};
static const FieldSpec _k_EffectTriggerMaster[] = {
  {0,"trigger","TriggerType",false,"enum",false},
  {1,"value","long",false,"prim",true}
};
static const FieldSpec _k_EnvironmentResult[] = {
  {0,"application_version","string",false,"prim",true},
  {1,"asset_version","string",false,"prim",true},
  {2,"api_endpoint","string",false,"prim",true},
  {3,"maintenance_api_endpoint","string",false,"prim",true},
  {4,"news_api_endpoint","string",false,"prim",true},
  {5,"is_maintenance","bool",false,"prim",false},
  {6,"master_data_url","string",false,"prim",true},
  {7,"static_content_url","string",false,"prim",true},
  {8,"asset_url","string",false,"prim",true},
  {9,"is_app_review","bool",false,"prim",false},
  {10,"photo_content_url","string",false,"prim",true},
  {11,"multi_real_time_server_url","string",false,"prim",true},
  {12,"external_payment_url","string",false,"prim",true}
};
static const FieldSpec _k_Episode[] = {
  {0,"episode_master_id","long",false,"prim",false},
  {1,"has_read_all","bool",false,"prim",false}
};
static const FieldSpec _k_EpisodeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_master_id","long",false,"prim",false},
  {2,"title","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"episode_reward_package_master_id","long",false,"prim",false},
  {5,"conditions","EpisodeReleaseCondition",true,"model",true},
  {6,"pre_episode_master_id","long",false,"prim",true},
  {7,"display_start_date","DateTime",false,"prim",true},
  {8,"display_end_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_EpisodeReleaseCondition[] = {
  {0,"condition_type","EpisodeReleaseConditionTypes",false,"enum",false},
  {1,"value1","long",false,"prim",true},
  {2,"value2","long",false,"prim",true},
  {3,"value3","long",false,"prim",true}
};
static const FieldSpec _k_EpisodeResult[] = {
  {0,"episode_title","string",false,"prim",true},
  {1,"story_type","StoryTypes",false,"enum",false},
  {2,"episode_order","int",false,"prim",false},
  {3,"episode_detail_asset_source","string",false,"prim",true}
};
static const FieldSpec _k_EpisodeDetailResult[] = {
  {0,"id_","long",false,"prim",false},
  {1,"episode_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"group_order","int",false,"prim",false},
  {4,"effect","string",false,"prim",true},
  {5,"speaker_name","string",false,"prim",true},
  {6,"font_size","FontSizes",false,"enum",false},
  {7,"phrase","string",false,"prim",true},
  {8,"title","string",false,"prim",true},
  {9,"background_image_file_name","string",false,"prim",true},
  {10,"background_character_image_file_name","string",false,"prim",true},
  {11,"background_image_file_fade_type","FadeTypes",false,"enum",true},
  {12,"bgm_file_name","string",false,"prim",true},
  {13,"se_file_name","string",false,"prim",true},
  {14,"still_photo_file_name","string",false,"prim",true},
  {15,"movie_file_name","string",false,"prim",true},
  {16,"window_effect","WindowEffects",false,"enum",true},
  {17,"scene_camera_master_id","long",false,"prim",true},
  {18,"voice_file_name","string",false,"prim",true},
  {19,"character_motions","EpisodeDetailCharacterMotionResult",true,"model",true},
  {20,"speaker_icon_id","string",false,"prim",true},
  {21,"fade_value1","float",false,"prim",true},
  {22,"fade_value2","float",false,"prim",true},
  {23,"fade_value3","float",false,"prim",true}
};
static const FieldSpec _k_EpisodeDetailCharacterMotionResult[] = {
  {0,"slot_number","int",false,"prim",false},
  {1,"facial_expression_master_id","long",false,"prim",true},
  {2,"head_motion_master_id","long",false,"prim",true},
  {3,"head_direction_master_id","long",false,"prim",true},
  {4,"body_motion_master_id","long",false,"prim",true},
  {5,"lip_sync_master_id","long",false,"prim",true},
  {6,"spine_id","long",false,"prim",false},
  {7,"character_appearance_type","CharacterAppearanceTypes",false,"enum",true},
  {8,"character_position","CharacterPositions",false,"enum",false},
  {9,"character_layer_type","CharacterLayerTypes",false,"enum",false},
  {10,"spine_size","SpineSizes",false,"enum",false}
};
static const FieldSpec _k_EpisodeRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","EpisodeRewardThing",true,"model",true}
};
static const FieldSpec _k_EpisodeRewardThing[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false},
  {3,"is_only_story_event_term","bool",false,"prim",false},
  {4,"is_chapter_all_read_reward","bool",false,"prim",false}
};
static const FieldSpec _k_Event[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"total_acquired_point","int",false,"prim",false},
  {3,"acquired_point_updated_date","DateTime",false,"prim",false},
  {4,"last_rank","int",false,"prim",true},
  {5,"read_tips","bool",false,"prim",false},
  {6,"login_days","int",false,"prim",false}
};
static const FieldSpec _k_EventBoxGacha[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_box_gacha_master_id","long",false,"prim",false},
  {2,"current_box_count","int",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaBoxThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_box_gacha_box_thing_master_id","long",false,"prim",false},
  {2,"hit_count","int",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaRollResult[] = {
  {0,"received_things","ReceivedThing",true,"model",true},
  {1,"has_reset","bool",false,"prim",false},
  {2,"drawn_event_box_gacha_box_thing_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_EventCamp[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"camp_type","CampTypes",false,"enum",false},
  {3,"total_support_point","int",false,"prim",false}
};
static const FieldSpec _k_EventResult[] = {
  {0,"acquired_event_point","int",false,"prim",true},
  {1,"attached_story_event_master_id","long",false,"prim",true},
  {2,"circle_event_high_score","long",false,"prim",true},
  {3,"circle_event_high_score_multiplier_percent","int",false,"prim",true},
  {4,"circle_event_before_high_score","long",false,"prim",true},
  {5,"high_score_before","long",false,"prim",true},
  {6,"high_score_after","long",false,"prim",true},
  {7,"before_rank","long",false,"prim",false},
  {8,"after_rank","long",false,"prim",false},
  {9,"story_event_master_id","long",false,"prim",true},
  {10,"event_master_id","long",false,"prim",true},
  {11,"this_time_support_point","int",false,"prim",true},
  {12,"camp_before_rank","long",false,"prim",true},
  {13,"camp_after_rank","long",false,"prim",true}
};
static const FieldSpec _k_ExchangeLimit[] = {
  {0,"id_","long",false,"prim",false},
  {1,"exchange_shop_thing_id","long",false,"prim",false},
  {2,"replace_type","ShopReplaceTypes",false,"enum",false},
  {3,"specified_number_of_days_limit","int",false,"prim",true},
  {4,"exchanged_count","int",false,"prim",false},
  {5,"until","DateTime",false,"prim",true}
};
static const FieldSpec _k_ExchangeShopMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"is_display_required_having_item","bool",false,"prim",false},
  {2,"category","ShopCategories",false,"enum",false},
  {3,"name","string",false,"prim",true},
  {4,"display_thing_type","ThingTypes",false,"enum",false},
  {5,"display_item_master_id","long",false,"prim",true},
  {6,"banner_path","string",false,"prim",true},
  {7,"start_date","DateTime",false,"prim",true},
  {8,"end_date","DateTime",false,"prim",true},
  {9,"last_refreshed_at","DateTime",false,"prim",false},
  {10,"lineup","ExchangeShopThing",true,"model",true},
  {11,"order","int",false,"prim",false}
};
static const FieldSpec _k_ExchangeShopThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"exchange_limit","int",false,"prim",true},
  {5,"order","int",false,"prim",false},
  {6,"replace_type","ShopReplaceTypes",false,"enum",true},
  {7,"is_display_locked","bool",false,"prim",false},
  {8,"unlock_type","ShopUnlockTypes",false,"enum",true},
  {9,"unlock_value","long",false,"prim",true},
  {10,"start_date","DateTime",false,"prim",true},
  {11,"end_date","DateTime",false,"prim",true},
  {12,"required_item_master_id","long",false,"prim",false},
  {13,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_ExchangeShopThingPayload[] = {
  {0,"m_exchange_shop_thing_id","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_FavoriteCostume[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"favorite_costume_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_FavoriteStampOrderPayload[] = {
  {0,"stamp_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_FinishAnotherNotationLivePayload[] = {
  {0,"base_score_blocks","BaseScoreBlock",true,"model",true},
  {1,"sense_score_blocks","SenseScoreBlock",true,"model",true},
  {2,"star_act_score_blocks","StarActScoreBlock",true,"model",true},
  {3,"multi_live_additional_score_blocks","MultiLiveAdditionalScoreBlock",true,"model",true},
  {4,"another_notation_master_id","long",false,"prim",false},
  {5,"is_auto_play","bool",false,"prim",false}
};
static const FieldSpec _k_FinishLessonPayload[] = {
  {0,"score","long",false,"prim",false},
  {1,"max_combo","int",false,"prim",false},
  {2,"judges","NoteJudgePayload",true,"model",true}
};
static const FieldSpec _k_FinishLivePayload[] = {
  {0,"score","long",false,"prim",false},
  {1,"max_combo","int",false,"prim",false},
  {2,"judges","NoteJudgePayload",true,"model",true},
  {3,"is_cleared","bool",false,"prim",false},
  {4,"base_score_blocks","BaseScoreBlock",true,"model",true},
  {5,"sense_score_blocks","SenseScoreBlock",true,"model",true},
  {6,"star_act_score_blocks","StarActScoreBlock",true,"model",true},
  {7,"multi_live_additional_score_blocks","MultiLiveAdditionalScoreBlock",true,"model",true},
  {8,"triple_cast_party_scores","TripleCastPartyScore",true,"model",true},
  {9,"trial_party_event_stage_result","TrialPartyEventStageResult",false,"model",true}
};
static const FieldSpec _k_FinishLiveResult[] = {
  {1,"league_rewards","ReceivedThing",true,"model",true},
  {2,"rate_result","RateResult",false,"model",true},
  {3,"player_rank_point_result","PlayerRankPointResult",false,"model",true},
  {5,"clear_lamp","ClearLamps",false,"enum",false},
  {6,"rate_grade","AchievementRateGrades",false,"enum",false},
  {8,"is_high_score","bool",false,"prim",false},
  {9,"audition_before_phase","byte",false,"prim",false},
  {10,"audition_after_phase","byte",false,"prim",false},
  {11,"audition_rewards","ReceivedThing",true,"model",true},
  {12,"story_event_rewards","ReceivedThing",true,"model",true},
  {13,"achievement_rate_average","double",false,"prim",false},
  {14,"audition_master_id","long",false,"prim",true},
  {16,"sp_rate_update_result","SpRateUpdateResult",false,"model",true},
  {17,"tournament_result","TournamentResult",false,"model",true},
  {18,"celling_max_count","int",false,"prim",true},
  {19,"celling_user_count","int",false,"prim",true},
  {20,"before_clear_lamp","ClearLamps",false,"enum",false},
  {21,"poster_drop_up_percent","int",false,"prim",false},
  {22,"event_result","EventResult",false,"model",true},
  {23,"live_drop_things","LiveDropThing",true,"model",true},
  {24,"lesson_result","LessonResult",false,"model",true},
  {25,"league_result","LeagueResult",false,"model",true},
  {26,"concert_result","ConcertResult",false,"model",true},
  {27,"bonus_live_result","BonusLiveResult",false,"model",true},
  {28,"ghost_live_result","GhostLiveResult",false,"model",true},
  {29,"trial_party_event_result","TrialPartyEventResult",false,"model",true},
  {30,"accessory_auto_sell_convert_things","AccessoryAutoSellConvertThing",true,"model",true}
};
static const FieldSpec _k_FlashSaleReadStagePayload[] = {
  {0,"read_flash_sale_stage_ids","long",true,"prim",true}
};
static const FieldSpec _k_FlashSaleStage[] = {
  {0,"id_","long",false,"prim",false},
  {1,"flash_sale_stage_master_id","long",false,"prim",false},
  {2,"purchase_limited_at","DateTime",false,"prim",true},
  {3,"is_default","bool",false,"prim",false},
  {4,"is_completed","bool",false,"prim",false}
};
static const FieldSpec _k_FriendAcceptResult[] = {
  {0,"result_status","FriendAcceptResultStatus",false,"enum",false}
};
static const FieldSpec _k_FriendFavoritePayload[] = {
  {0,"target_user_id","string",false,"prim",true},
  {1,"set_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_FriendInvitation[] = {
  {0,"id_","long",false,"prim",false},
  {1,"invitation_code","string",false,"prim",true},
  {2,"has_input_other_invitation_code","bool",false,"prim",false}
};
static const FieldSpec _k_FriendInvitationMission[] = {
  {0,"id_","long",false,"prim",false},
  {1,"friend_invitation_mission_master_id","long",false,"prim",false},
  {2,"friend_invitation_mission_stage_master_id","long",false,"prim",false},
  {3,"current_count","int",false,"prim",false},
  {4,"is_cleared","bool",false,"prim",false},
  {5,"is_reward_received","bool",false,"prim",false}
};
static const FieldSpec _k_FriendInvitationMissionPayload[] = {
  {0,"friend_invitation_mission_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_FriendInvitationUserInfoResult[] = {
  {0,"hashed_user_id","string",false,"prim",true},
  {1,"name","string",false,"prim",true},
  {2,"player_rank","long",false,"prim",false},
  {3,"result_status","InvitationCodeResultStatuses",false,"enum",false}
};
static const FieldSpec _k_FriendListResult[] = {
  {0,"results","FriendResult",true,"model",true},
  {1,"current_friend_count","int",false,"prim",false}
};
static const FieldSpec _k_FriendRequest[] = {
  {0,"request_user_id","string",false,"prim",true},
  {1,"received_user_id","string",false,"prim",true},
  {2,"request_user_name","string",false,"prim",true}
};
static const FieldSpec _k_FriendRequestResult[] = {
  {0,"result_status","FriendRequestResultStatus",false,"enum",false}
};
static const FieldSpec _k_FriendResult[] = {
  {0,"user_id","string",false,"prim",true},
  {2,"player_rank","int",false,"prim",false},
  {3,"trophy_master_id1","long",false,"prim",false},
  {4,"trophy_master_id2","long",false,"prim",false},
  {5,"trophy_master_id3","long",false,"prim",false},
  {6,"introduction","string",false,"prim",true},
  {7,"last_logged_in_at","DateTime",false,"prim",false},
  {8,"name","string",false,"prim",true},
  {9,"player_rate","double",false,"prim",true},
  {10,"is_public_player_rate","bool",false,"prim",false},
  {11,"league_class","LeagueClassTypes",false,"enum",false},
  {12,"character_ranks","CharacterRank",true,"model",true},
  {13,"is_public_album_main_page","bool",false,"prim",false},
  {15,"main_character_master_id","long",false,"prim",false},
  {16,"display_awakening_status","bool",false,"prim",false},
  {17,"icon_frame_master_id","long",false,"prim",false},
  {18,"is_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_FriendSearchResult[] = {
  {0,"friend_result","FriendResult",false,"model",true},
  {1,"result_status","FriendSearchResultStatus",false,"enum",false}
};
static const FieldSpec _k_Gacha[] = {
  {0,"id_","long",false,"prim",false},
  {1,"gacha_master_id","long",false,"prim",false},
  {2,"roll_count","int",false,"prim",false}
};
static const FieldSpec _k_GachaHistoryResult[] = {
  {0,"master_id","long",false,"prim",false},
  {1,"created_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_GachaInfoResult[] = {
  {0,"id_","long",false,"prim",false},
  {1,"roll_limits","GachaRollLimit",true,"model",true},
  {2,"normal_emission_flags","GachaEmissionFlags",false,"enum",false},
  {3,"fixed_emission_flags","GachaEmissionFlags",false,"enum",false}
};
static const FieldSpec _k_GachaLineupItemProbability[] = {
  {0,"id_","long",false,"prim",false},
  {2,"probability","double",false,"prim",false}
};
static const FieldSpec _k_GachaReRoll[] = {
  {0,"gacha_master_id","long",false,"prim",false},
  {1,"roll_count","int",false,"prim",false},
  {2,"is_decided","bool",false,"prim",false}
};
static const FieldSpec _k_GachaRollLimit[] = {
  {0,"gacha_detail_master_id","long",false,"prim",false},
  {1,"roll_left","int",false,"prim",false}
};
static const FieldSpec _k_GachaRollResult[] = {
  {0,"m_gacha_master_id","long",false,"prim",false},
  {1,"received_things","GachaThingResult",true,"model",true},
  {2,"received_bonus_things","ReceivedThing",true,"model",true},
  {3,"received_detail_bonus_things","ReceivedThing",true,"model",true},
  {4,"received_roll_bonus_things","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_GachaSelectedThing[] = {
  {0,"gacha_master_id","long",false,"prim",false},
  {1,"gacha_thing_ids","long",true,"prim",true}
};
static const FieldSpec _k_GachaSelectedThingsResult[] = {
  {0,"selected_gacha_thing_ids","long",true,"prim",true}
};
static const FieldSpec _k_GachaThingResult[] = {
  {0,"received_things","ReceivedThing",true,"model",true},
  {1,"additional_received_things","ReceivedThing",true,"model",true},
  {2,"movie_costume_master_id","long",false,"prim",true}
};
static const FieldSpec _k_GameHint[] = {
  {0,"id_","long",false,"prim",false},
  {1,"page_category","PageCategories",false,"enum",false},
  {2,"has_already_read","bool",false,"prim",false}
};
static const FieldSpec _k_GameHintMonoBehaviour[] = {
};
static const FieldSpec _k_GenerateLotteryResultPayload[] = {
  {0,"prize","int",false,"prim",false},
  {1,"amount","int",false,"prim",false}
};
static const FieldSpec _k_GeneratePhotoPayload[] = {
  {0,"character_base_master_id","long",false,"prim",true},
  {1,"item_master_id","long",false,"prim",false},
  {2,"image_data","byte",true,"prim",true},
  {3,"thumbnail_image_data","byte",true,"prim",true},
  {4,"appeared_characters","PhotoAppearedCharacter",true,"model",true}
};
static const FieldSpec _k_GeneratePhotoResult[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"file_name","string",false,"prim",true},
  {2,"sas_token","string",false,"prim",true},
  {3,"rarity","PhotoRarities",false,"enum",false},
  {4,"sign_master_id","long",false,"prim",true},
  {5,"photo_effect_master_id","long",false,"prim",true}
};
static const FieldSpec _k_GeneratePhotosPayload[] = {
  {0,"payloads","GeneratePhotoPayload",true,"model",true}
};
static const FieldSpec _k_GhostLiveInfo[] = {
  {0,"leader_position","int",false,"prim",false},
  {1,"party_slot","PartySlotDetail",true,"model",true}
};
static const FieldSpec _k_GhostLiveResult[] = {
  {0,"ghost_user_profile","UserProfileDetail",false,"model",true},
  {1,"ghost_user_score","long",false,"prim",false},
  {2,"total_battle_win_count","long",false,"prim",false},
  {3,"matching_result","MatchingResult",false,"enum",false},
  {4,"display_character_master_id","long",false,"prim",false},
  {5,"display_character_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_GradualMissionGroup[] = {
  {0,"id_","long",false,"prim",false},
  {1,"gradual_mission_group_master_id","long",false,"prim",false},
  {2,"start_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_HighScoreBuff[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"buffs","HighScoreBuffSetting",true,"model",true}
};
static const FieldSpec _k_HighScoreBuffSetting[] = {
  {0,"story_event_high_score_buff_setting_id","long",false,"prim",false},
  {1,"current_level","int",false,"prim",false}
};
static const FieldSpec _k_HighScoreParty[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"user_name","string",false,"prim",true},
  {2,"party_name","string",false,"prim",true},
  {3,"slots","HighScorePartySlot",true,"model",true},
  {4,"difficulty","MusicDifficulties",false,"enum",false},
  {5,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_HighScorePartySlot[] = {
  {0,"position","int",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"character_level","int",false,"prim",false},
  {3,"character_talent_stage","int",false,"prim",false},
  {4,"character_awakening_phase","int",false,"prim",false},
  {5,"poster_master_id","long",false,"prim",true},
  {6,"poster_level","int",false,"prim",true},
  {7,"poster_breakthrough_phase","int",false,"prim",true},
  {8,"accessory_master_id","long",false,"prim",true},
  {9,"accessory_level","int",false,"prim",true},
  {10,"current_status","Status",false,"model",true},
  {11,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_HomeBGM[] = {
  {0,"home_b_g_m_master_id","long",false,"prim",false},
  {1,"selection_type","HomeBGMSelectionTypes",false,"enum",false},
  {2,"home_b_g_m_detail_master_id","long",false,"prim",true}
};
static const FieldSpec _k_HomeCharacterVoiceMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"text","string",false,"prim",true},
  {3,"weight","int",false,"prim",false},
  {5,"character_voice_period_master_id","long",false,"prim",true},
  {7,"is_player_birth_date_voice","bool",false,"prim",false},
  {8,"voice_file_name1","string",false,"prim",true},
  {9,"voice_file_name2","string",false,"prim",true},
  {10,"voice_file_name3","string",false,"prim",true},
  {11,"voice_file_name4","string",false,"prim",true},
  {13,"voice_interval1","float",false,"prim",false},
  {14,"voice_interval2","float",false,"prim",false},
  {15,"voice_interval3","float",false,"prim",false},
  {16,"mouth_motion_id1","string",false,"prim",true},
  {17,"mouth_motion_id2","string",false,"prim",true},
  {18,"mouth_motion_id3","string",false,"prim",true},
  {19,"mouth_motion_id4","string",false,"prim",true},
  {20,"body_motion_id1","string",false,"prim",true},
  {21,"body_motion_id2","string",false,"prim",true},
  {22,"body_motion_id3","string",false,"prim",true},
  {23,"body_motion_id4","string",false,"prim",true}
};
static const FieldSpec _k_HomeDisplayPreference[] = {
  {0,"id_","long",false,"prim",false},
  {1,"home_character_base_master_id","long",false,"prim",true},
  {3,"member_character_base_master_id","long",false,"prim",true},
  {4,"story_character_base_master_id","long",false,"prim",true},
  {5,"shop_character_base_master_id","long",false,"prim",true},
  {6,"home_costume_master_id","long",false,"prim",true},
  {8,"member_costume_master_id","long",false,"prim",true},
  {9,"story_costume_master_id","long",false,"prim",true},
  {10,"shop_costume_master_id","long",false,"prim",true},
  {11,"illust_character_master_id","long",false,"prim",false},
  {12,"display_awakening_status","bool",false,"prim",false},
  {13,"home_character_display_type","HomeCharacterDisplayTypes",false,"enum",false},
  {14,"login_bonus_character_base_master_id","long",false,"prim",true},
  {15,"login_bonus_costume_master_id","long",false,"prim",true}
};
static const FieldSpec _k_HomeSkin[] = {
  {0,"id_","long",false,"prim",false},
  {1,"home_skin_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_IconFrame[] = {
  {0,"id_","long",false,"prim",false},
  {1,"icon_frame_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_Inbox[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"is_time_limited","bool",false,"prim",false},
  {5,"has_received","bool",false,"prim",false},
  {6,"title","string",false,"prim",true},
  {7,"description","string",false,"prim",true},
  {8,"sent_at","DateTime",false,"prim",false},
  {9,"received_at","DateTime",false,"prim",true},
  {10,"receive_limit_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_InboxReceiveResult[] = {
  {0,"received_things","ReceivedThing",true,"model",true},
  {1,"has_not_receive_things","bool",false,"prim",false}
};
static const FieldSpec _k_InviteMultiRoomPayload[] = {
  {0,"hashed_user_ids","string",true,"prim",true}
};
static const FieldSpec _k_Item[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"stock","int",false,"prim",false}
};
static const FieldSpec _k_ItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"display_order","long",false,"prim",false},
  {4,"display_end_date","DateTime",false,"prim",true},
  {5,"max_stock","int",false,"prim",false},
  {6,"category","ItemCategories",false,"enum",false},
  {7,"consumable","bool",false,"prim",false},
  {8,"jump_type","JumpTypes",false,"enum",true},
  {9,"jump_target_id","long",false,"prim",true},
  {10,"tab_category","TabCategories",false,"enum",false},
  {11,"rarity","PossessionRarities",false,"enum",false}
};
static const FieldSpec _k_JewelShop[] = {
  {0,"id_","long",false,"prim",false},
  {1,"jewel_shop_item_master_id","long",false,"prim",false},
  {2,"purchase_count","int",false,"prim",false},
  {3,"total_purchase_count","int",false,"prim",false},
  {4,"re_purchase_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_JoinMultiRoomPayload[] = {
  {0,"hashed_room_id","string",false,"prim",true},
  {1,"password","string",false,"prim",true}
};
static const FieldSpec _k_LeagueBasic[] = {
  {0,"my_property","long",false,"prim",false},
  {1,"star_enroll_count","int",false,"prim",false},
  {2,"dai_star_enroll_count","int",false,"prim",false},
  {3,"current_class_type","LeagueClassTypes",false,"enum",false},
  {4,"best_class_type","LeagueClassTypes",false,"enum",false},
  {5,"last_joined_league_season_master_id","long",false,"prim",true}
};
static const FieldSpec _k_LeagueGroup[] = {
  {0,"league_master_id","long",false,"prim",false},
  {1,"class_type","LeagueClassTypes",false,"enum",false},
  {2,"class_order","int",false,"prim",false}
};
static const FieldSpec _k_LeagueGroupMember[] = {
  {0,"league_group_id","long",false,"prim",false},
  {1,"league_master_id","long",false,"prim",false},
  {2,"best_score","long",false,"prim",true}
};
static const FieldSpec _k_LeagueHighScoreParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"league_master_id","long",false,"prim",false},
  {2,"high_score","long",false,"prim",false},
  {3,"class_type","LeagueClassTypes",false,"enum",false},
  {4,"difficulty","MusicDifficulties",false,"enum",false},
  {5,"music_master_id","long",false,"prim",false},
  {6,"league_group_id","long",false,"prim",false},
  {7,"slots","LeagueHighScorePartySlot",true,"model",true},
  {8,"user_name","string",false,"prim",true},
  {9,"acting_ability","int",false,"prim",false},
  {10,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_LeagueHighScorePartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"league_high_score_party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"character_master_id","long",false,"prim",false},
  {4,"character_level","int",false,"prim",false},
  {7,"poster_master_id","long",false,"prim",true},
  {8,"poster_level","int",false,"prim",true},
  {9,"poster_breakthrough_phase","int",false,"prim",true},
  {10,"accessory_master_id","long",false,"prim",true},
  {11,"accessory_level","int",false,"prim",true},
  {12,"current_status","Status",false,"model",true},
  {13,"character_talent_stage","int",false,"prim",false},
  {14,"character_awakening_phase","int",false,"prim",false},
  {15,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_LeagueHistory[] = {
  {0,"league_master_id","long",false,"prim",false},
  {1,"class_type","LeagueClassTypes",false,"enum",false},
  {2,"history_count","int",false,"prim",false},
  {3,"is_sended_reward","bool",false,"prim",false},
  {5,"is_played","bool",false,"prim",false},
  {6,"class_change_type","LeagueClassChangeTypes",false,"enum",false},
  {8,"group_rank","int",false,"prim",false},
  {9,"global_rank","int",false,"prim",false},
  {10,"all_class_global_rank","int",false,"prim",false}
};
static const FieldSpec _k_LeaguePartyAndRankingResult[] = {
  {0,"rank","int",false,"prim",true},
  {1,"user_id","string",false,"prim",true},
  {2,"best_score","long",false,"prim",false},
  {3,"league_master_id","long",false,"prim",false},
  {4,"class_type","LeagueClassTypes",false,"enum",false},
  {5,"user_name","string",false,"prim",true},
  {6,"acting_ability","int",false,"prim",false},
  {7,"slots","LeaguePartySlotResult",true,"model",true},
  {8,"difficulty","MusicDifficulties",false,"enum",false}
};
static const FieldSpec _k_LeaguePartySlotResult[] = {
  {0,"slot_position","int",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"attribute","Attributes",false,"enum",false},
  {4,"character_rarity","CharacterRarities",false,"enum",false},
  {6,"character_level","int",false,"prim",false},
  {7,"poster_master_id","long",false,"prim",true},
  {8,"poster_rarity","PossessionRarities",false,"enum",true},
  {9,"poster_level","int",false,"prim",true},
  {10,"poster_breakthrough_phase","int",false,"prim",true},
  {11,"accessory_master_id","long",false,"prim",true},
  {12,"accessory_rarity","PossessionRarities",false,"enum",true},
  {13,"accessory_level","int",false,"prim",true},
  {14,"current_status","Status",false,"model",true},
  {15,"position","int",false,"prim",false},
  {16,"character_talent_stage","int",false,"prim",false},
  {17,"character_awakening_phase","int",false,"prim",false},
  {18,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_LeagueReceiveResults[] = {
  {0,"class_reward_received_things","ReceivedThing",true,"model",true},
  {1,"class_reward_type","LeagueClassChangeTypes",false,"enum",false},
  {2,"first_achieve_received_things","ReceivedThing",true,"model",true},
  {3,"group_rank_received_things","ReceivedThing",true,"model",true},
  {4,"is_first_time_enrolled_class","bool",false,"prim",false},
  {5,"all_class_global_rank_received_things","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_LeagueResult[] = {
  {0,"before_group_ranking","int",false,"prim",true},
  {1,"after_group_ranking","int",false,"prim",false},
  {2,"lowest_up_border_score","long",false,"prim",true},
  {3,"lowest_keep_border_score","long",false,"prim",true},
  {4,"enrolled_group_number","int",false,"prim",false},
  {5,"is_in_counting","bool",false,"prim",false}
};
static const FieldSpec _k_LeagueSeasonResult[] = {
  {0,"league_season_master_id","long",false,"prim",false},
  {1,"dai_star_max_enroll_count","int",false,"prim",false}
};
static const FieldSpec _k_LeagueTopMenuInformationResult[] = {
  {0,"display_start_at","DateTime",false,"prim",false},
  {1,"counting_start_at","DateTime",false,"prim",false},
  {2,"display_end_at","DateTime",false,"prim",false},
  {3,"music_master_id","long",false,"prim",false},
  {6,"league_group_ranking","int",false,"prim",false},
  {8,"up_class_border_score","long",false,"prim",true},
  {9,"up_class_border_ranking","int",false,"prim",true},
  {10,"keep_class_border_score","long",false,"prim",true},
  {11,"keep_class_border_ranking","int",false,"prim",true},
  {12,"group_lowest_rank","int",false,"prim",true},
  {13,"is_participated","bool",false,"prim",false},
  {14,"enrolled_group_number","int",false,"prim",true}
};
static const FieldSpec _k_LessonResult[] = {
  {0,"character_base_master_id","long",false,"prim",false},
  {1,"star_point_result","StarPointResult",false,"model",true},
  {2,"high_score_rewards","ReceivedThing",true,"model",true},
  {3,"high_score_before","long",false,"prim",false},
  {4,"high_score_after","long",false,"prim",false}
};
static const FieldSpec _k_LevelUpPhotoPayload[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"after_level","int",false,"prim",false}
};
static const FieldSpec _k_LevelUpPhotoResult[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"before_level","int",false,"prim",false},
  {2,"after_level","int",false,"prim",false}
};
static const FieldSpec _k_Limit[] = {
  {0,"id_","long",false,"prim",false},
  {1,"additional_acquirable_photo_limit","int",false,"prim",false},
  {2,"acquirable_photo_limit_increased_times","int",false,"prim",false},
  {3,"additional_acquirable_accessory_limit","int",false,"prim",false},
  {4,"acquirable_accessory_limit_increased_times","int",false,"prim",false}
};
static const FieldSpec _k_LinkCharacter[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"company_master_id","long",false,"prim",false},
  {3,"linked_character_base_master_id","long",false,"prim",true},
  {4,"reward_received_max_rank","int",false,"prim",false}
};
static const FieldSpec _k_Live[] = {
  {0,"id_","long",false,"prim",false},
  {1,"live_master_id","long",false,"prim",false},
  {2,"times_completed","int",false,"prim",false},
  {4,"achievement_rate","float",false,"prim",false},
  {5,"notation_rate","float",false,"prim",false},
  {6,"clear_lamp","ClearLamps",false,"enum",false},
  {7,"status","LiveReleaseStatus",false,"enum",false},
  {8,"rate_grade","AchievementRateGrades",false,"enum",false}
};
static const FieldSpec _k_LiveAchievement[] = {
  {0,"id_","long",false,"prim",false},
  {1,"olivier_released_count","int",false,"prim",false},
  {2,"olivier_cleared_level","int",false,"prim",false}
};
static const FieldSpec _k_LiveDropCelling[] = {
  {0,"id_","long",false,"prim",false},
  {1,"multi_live_schedule_master_id","long",false,"prim",false},
  {2,"count","int",false,"prim",false},
  {3,"total_celling_count","int",false,"prim",false}
};
static const FieldSpec _k_LiveDropLimit[] = {
  {0,"multi_live_schedule_master_id","long",false,"prim",false},
  {1,"current_count","int",false,"prim",false},
  {2,"count_limit","int",false,"prim",false}
};
static const FieldSpec _k_LiveDropThing[] = {
  {0,"received_thing","ReceivedThing",false,"model",true},
  {1,"order","long",false,"prim",false},
  {2,"live_drop_type","LiveDropTypes",false,"enum",false},
  {3,"is_fixed_drop","bool",false,"prim",false}
};
static const FieldSpec _k_LiveMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"difficulty","MusicDifficulties",false,"enum",false},
  {2,"music_master_id","long",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"note_count","int",false,"prim",false},
  {5,"unlock_condition","LiveUnlockConditionTypes",false,"enum",false},
  {6,"unlock_value","long",false,"prim",true},
  {7,"start_date","DateTime",false,"prim",false},
  {8,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_LiveSettingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"live_type","LiveTypes",false,"enum",false},
  {3,"live_drop_frame_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_LiveTimeEvent[] = {
  {0,"timings","SenseTimingEvent",true,"model",true},
  {1,"cool_times","SenseCoolTime",true,"model",true}
};
static const FieldSpec _k_LiveUnit[] = {
  {0,"actors","Dictionary",false,"model",true},
  {1,"time_events","Dictionary",false,"model",true},
  {2,"possible_senses","Sense",true,"model",true},
  {3,"start_effects","Effect",true,"model",true},
  {4,"star_act","StarAct",false,"model",true},
  {5,"total_status","int",false,"prim",false},
  {6,"star_act_sense_light_count","int",false,"prim",false},
  {7,"max_principal","int",false,"prim",false},
  {8,"is_first_play_olivier","bool",false,"prim",false},
  {9,"base_score_percentage","int",false,"prim",false},
  {10,"base_score_difficulty_auto_coefficient","double",false,"prim",false},
  {11,"u_active_live_id","long",false,"prim",false}
};
static const FieldSpec _k_LiveUnitWithOrder[] = {
  {0,"actors","Dictionary",false,"model",true},
  {1,"time_events","Dictionary",false,"model",true},
  {2,"possible_senses","Sense",true,"model",true},
  {3,"start_effects","Effect",true,"model",true},
  {4,"star_act","StarAct",false,"model",true},
  {5,"total_status","int",false,"prim",false},
  {6,"star_act_sense_light_count","int",false,"prim",false},
  {7,"max_principal","int",false,"prim",false},
  {8,"is_first_play_olivier","bool",false,"prim",false},
  {9,"base_score_percentage","int",false,"prim",false},
  {10,"base_score_difficulty_auto_coefficient","double",false,"prim",false},
  {11,"u_active_live_id","long",false,"prim",false},
  {12,"order","int",false,"prim",false}
};
static const FieldSpec _k_LoginBonusDetail[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false},
  {3,"login_count","int",false,"prim",false}
};
static const FieldSpec _k_LoginBonusResult[] = {
  {0,"login_bonus_id","long",false,"prim",false},
  {1,"current_login_count","int",false,"prim",false},
  {2,"received_things","ReceivedThing",true,"model",true},
  {3,"title","string",false,"prim",true},
  {4,"type","LoginBonusTypes",false,"enum",false},
  {5,"start_date","DateTime",false,"prim",false},
  {6,"end_date","DateTime",false,"prim",true},
  {10,"message_template","string",false,"prim",true},
  {11,"background_image_path","string",false,"prim",true},
  {12,"cardboard_image_path","string",false,"prim",true},
  {13,"logo_image_path","string",false,"prim",true},
  {14,"total_days","int",false,"prim",false},
  {15,"is_loop","bool",false,"prim",false},
  {16,"order","int",false,"prim",false},
  {17,"details","LoginBonusDetail",true,"model",true},
  {18,"navigation_spine_id","string",false,"prim",true},
  {19,"voice_asset_id","string",false,"prim",true},
  {20,"head_motion_master_id","long",false,"prim",true},
  {21,"facial_expression_master_id","long",false,"prim",true},
  {22,"head_direction_master_id","long",false,"prim",true},
  {23,"body_motion_master_id","long",false,"prim",true},
  {24,"lip_sync_master_id","long",false,"prim",true},
  {25,"layout_type","LoginBonusLayoutTypes",false,"enum",false},
  {26,"comeback_campaign_master_id","long",false,"prim",true},
  {27,"login_bonus_spine_group","LoginBonusSpineGroup",false,"model",true}
};
static const FieldSpec _k_LoginBonusSpineDetail[] = {
  {0,"target_value1","long",false,"prim",true},
  {1,"target_value2","long",false,"prim",true},
  {2,"message_template","string",false,"prim",true},
  {3,"navigation_spine_id","string",false,"prim",true},
  {4,"voice_asset_id","string",false,"prim",true},
  {5,"head_motion_master_id","long",false,"prim",true},
  {6,"facial_expression_master_id","long",false,"prim",true},
  {7,"head_direction_master_id","long",false,"prim",true},
  {8,"body_motion_master_id","long",false,"prim",true},
  {9,"lip_sync_master_id","long",false,"prim",true}
};
static const FieldSpec _k_LoginBonusSpineGroup[] = {
  {0,"spine_select_type","LoginBonusSpineSelectType",false,"enum",false},
  {1,"details","LoginBonusSpineDetail",true,"model",true}
};
static const FieldSpec _k_LoginPassStatus[] = {
  {0,"id_","long",false,"prim",false},
  {1,"valid_until","DateTime",false,"prim",false}
};
static const FieldSpec _k_LoginPayload[] = {
  {0,"push_notification_token","string",false,"prim",true}
};
static const FieldSpec _k_LoginResult[] = {
  {0,"invalided_star_passes","StarPassTypes",true,"enum",true},
  {1,"login_pass_notification","LoginPassNotificationTypes",false,"enum",false},
  {2,"is_approaching_login_pass_invalided","bool",false,"prim",false},
  {3,"invalided_item_master_ids","long",true,"prim",true},
  {4,"approaching_item_master_ids","long",true,"prim",true},
  {5,"story_event_point_exchange_result","StoryEventPointExchangeResult",true,"model",true},
  {6,"invalided_buff_item_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_Lottery[] = {
  {0,"lottery_master_id","long",false,"prim",false},
  {1,"results","LotteryPriseResult",true,"model",true}
};
static const FieldSpec _k_LotteryPriseResult[] = {
  {0,"prize","int",false,"prim",false},
  {1,"amount","int",false,"prim",false}
};
static const FieldSpec _k_MainCharacter[] = {
  {0,"character_master_id","long",false,"prim",false},
  {1,"awakening_phase","int",false,"prim",false},
  {2,"talent_stage","int",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_Market[] = {
  {0,"id_","long",false,"prim",false},
  {1,"last_refreshed_at","DateTime",false,"prim",false},
  {2,"refresh_times","int",false,"prim",false}
};
static const FieldSpec _k_MarketResult[] = {
  {0,"things","MarketThing",true,"model",true},
  {1,"required_jewel_for_refresh","int",false,"prim",true}
};
static const FieldSpec _k_MarketThing[] = {
  {0,"frame_number","long",false,"prim",false},
  {1,"market_frame_thing_master_id","long",false,"prim",false},
  {4,"has_purchased","bool",false,"prim",false},
  {5,"discount_percent","int",false,"prim",true}
};
static const FieldSpec _k_MarshalByRefObject[] = {
};
static const FieldSpec _k_MasterDataManifest[] = {
  {0,"uri","string",false,"prim",true},
  {1,"sas_token","string",false,"prim",true},
  {2,"version","string",false,"prim",true},
  {3,"publish_timestamp","long",false,"prim",false}
};
static const FieldSpec _k_MatchingGhostLiveResult[] = {
  {0,"ghost_live_master_id","long",false,"prim",false},
  {1,"ghost_user_profile","UserProfileDetail",false,"model",true},
  {2,"ghost_live_info","GhostLiveInfo",false,"model",true}
};
static const FieldSpec _k_Mission[] = {
  {0,"id_","long",false,"prim",false},
  {1,"is_cleared","bool",false,"prim",false},
  {2,"is_reward_received","bool",false,"prim",false},
  {3,"mission_current_count","long",false,"prim",false},
  {5,"mission_master_id","long",false,"prim",false},
  {6,"current_mission_stage_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MissionCleared[] = {
  {0,"mission_master_id","long",false,"prim",false},
  {1,"mission_stage_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"mission_category","MissionCategories",false,"enum",false},
  {2,"mission_view_order","int",false,"prim",false},
  {3,"title","string",false,"prim",true},
  {4,"description","string",false,"prim",true},
  {11,"event_master_id","long",false,"prim",true},
  {12,"jump_type","JumpTypes",false,"enum",false},
  {13,"jump_target_id","long",false,"prim",true},
  {14,"start_date","DateTime",false,"prim",false},
  {15,"end_date","DateTime",false,"prim",false},
  {16,"stages","MissionStageMaster",true,"model",true},
  {17,"comeback_campaign_master_id","long",false,"prim",true}
};
static const FieldSpec _k_MissionPass[] = {
  {0,"id_","long",false,"prim",false},
  {1,"user_id","long",false,"prim",false},
  {2,"mission_pass_master_id","long",false,"prim",false},
  {3,"paid","bool",false,"prim",false},
  {4,"free_reward_received_phase","int",false,"prim",true},
  {5,"sp_reward_received_phase","int",false,"prim",true},
  {6,"terminated","bool",false,"prim",false},
  {7,"free_reward_loop_count","int",false,"prim",true},
  {8,"free_reward_loop_received_phase","int",false,"prim",true},
  {9,"paid_reward_loop_count","int",false,"prim",true},
  {10,"paid_reward_loop_received_phase","int",false,"prim",true}
};
static const FieldSpec _k_MissionPassDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"phase","int",false,"prim",false},
  {2,"mission_pass_master_id","long",false,"prim",true},
  {3,"clear_point","int",false,"prim",false},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false},
  {6,"rewards","MissionPassRewardThing",true,"model",true}
};
static const FieldSpec _k_MissionPassMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_MissionPassRewardThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"mission_pass_detail_master_id","long",false,"prim",false},
  {5,"is_sp","bool",false,"prim",false}
};
static const FieldSpec _k_MissionPassRewardsResult[] = {
  {0,"mission_pass_rewards","ReceivedThing",true,"model",true},
  {1,"reward_result","MissionPassRewardStatus",false,"enum",false}
};
static const FieldSpec _k_MissionRewardMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_MissionStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"mission_stage_order","int",false,"prim",false},
  {2,"stage_goal_value","long",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",true},
  {4,"rewards","MissionRewardMaster",true,"model",true}
};
static const FieldSpec _k_MonoBehaviour[] = {
};
static const FieldSpec _k_MultiLiveAdditionalScoreBlock[] = {
  {0,"hash","long",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"life","int",false,"prim",false},
  {3,"time_event_second","int",false,"prim",false},
  {4,"combo","int",false,"prim",false}
};
static const FieldSpec _k_MultiLiveInformation[] = {
  {0,"scores","Dictionary",false,"model",true}
};
static const FieldSpec _k_MultiLiveRestriction[] = {
  {0,"restriction_finished_at","DateTime",false,"prim",true}
};
static const FieldSpec _k_MultiRoomBasic[] = {
  {0,"id_","long",false,"prim",false},
  {1,"owner_multi_room_id","string",false,"prim",true}
};
static const FieldSpec _k_MultiRoomCreateResult[] = {
  {0,"hashed_multi_room_id","string",false,"prim",true},
  {1,"room_detail","MultiRoomDetailResult",false,"model",true}
};
static const FieldSpec _k_MultiRoomDetailResult[] = {
  {0,"multi_room_information_result","MultiRoomInformationResult",false,"model",true},
  {1,"multi_room_rankings","MultiRoomRanking",true,"model",true},
  {2,"is_finished","bool",false,"prim",false},
  {3,"password","string",false,"prim",true}
};
static const FieldSpec _k_MultiRoomInformationResult[] = {
  {0,"hashed_multi_room_id","string",false,"prim",true},
  {1,"room_name","string",false,"prim",true},
  {2,"play_mode","MultiRoomPlayModes",false,"enum",false},
  {3,"joined_member_count","int",false,"prim",false},
  {4,"can_join_max_member","int",false,"prim",false},
  {5,"start_date","DateTime",false,"prim",false},
  {6,"end_date","DateTime",false,"prim",false},
  {7,"live_master_id1","long",false,"prim",false},
  {8,"live_master_id2","long",false,"prim",true},
  {9,"live_master_id3","long",false,"prim",true},
  {10,"has_password","bool",false,"prim",false}
};
static const FieldSpec _k_MultiRoomInvitedResult[] = {
  {0,"owner_user_name","string",false,"prim",true},
  {1,"multi_room_information_result","MultiRoomInformationResult",false,"model",true}
};
static const FieldSpec _k_MultiRoomJoinnedResult[] = {
  {0,"my_rank","int",false,"prim",false},
  {1,"my_score","long",false,"prim",true},
  {2,"my_total_achievement_rate_percent_record","Decimal",false,"model",true},
  {3,"multi_room_information_result","MultiRoomInformationResult",false,"model",true}
};
static const FieldSpec _k_MultiRoomPartySlot[] = {
  {0,"position","int",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"character_level","int",false,"prim",false},
  {3,"character_talent_stage","int",false,"prim",false},
  {4,"character_awakening_phase","int",false,"prim",false},
  {5,"character_display_awakening_status","bool",false,"prim",false},
  {6,"poster_master_id","long",false,"prim",true},
  {7,"poster_level","int",false,"prim",true},
  {8,"poster_breakthrough_phase","int",false,"prim",true},
  {9,"accessory_master_id","long",false,"prim",true},
  {10,"accessory_level","int",false,"prim",true},
  {11,"u_accessory_id","long",false,"prim",true},
  {12,"current_status","Status",false,"model",true}
};
static const FieldSpec _k_MultiRoomRanking[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"rank","int",false,"prim",false},
  {2,"is_owner","bool",false,"prim",false},
  {3,"score","long",false,"prim",true},
  {4,"total_achievement_rate_percent_record","Decimal",false,"prim",true},
  {5,"best_record_challenge_count","int",false,"prim",false},
  {6,"best_record_date","DateTime",false,"prim",true},
  {7,"user_profile","UserProfile",false,"model",true},
  {8,"party_info","PartyInfo",false,"model",true},
  {9,"note_result","NoteResult",false,"model",true}
};
static const FieldSpec _k_Music[] = {
  {0,"id_","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false},
  {5,"stella_released","bool",false,"prim",false},
  {7,"vocal_version","int",false,"prim",false},
  {8,"olivier_release_status","OlivierReleaseStatuses",false,"enum",false},
  {9,"is_possession","bool",false,"prim",false}
};
static const FieldSpec _k_MusicBookmark[] = {
  {0,"music_master_id","long",false,"prim",false},
  {1,"music_bookmark_flag","MusicBookmarkFlags",false,"enum",false}
};
static const FieldSpec _k_MusicCourse[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_course_master_id","long",false,"prim",false},
  {2,"clear_lamp","ClearLamps",false,"enum",false},
  {3,"certification_grade","MusicCourseCertificationGrade",false,"enum",false},
  {4,"total_achievement_rate_percent_record","Decimal",false,"prim",true}
};
static const FieldSpec _k_MusicCourseRandomSelectResult[] = {
  {0,"details","MusicCourseRandomSelectResultDetail",true,"model",true}
};
static const FieldSpec _k_MusicCourseRandomSelectResultDetail[] = {
  {0,"set_list_number","int",false,"prim",false},
  {1,"live_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MusicCourseRanking[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_course_master_id","long",false,"prim",false},
  {2,"current_challenge_count","int",false,"prim",false},
  {3,"perfect_star","int",false,"prim",false},
  {4,"perfect","int",false,"prim",false},
  {5,"great","int",false,"prim",false},
  {6,"good","int",false,"prim",false},
  {7,"bad","int",false,"prim",false},
  {8,"miss","int",false,"prim",false},
  {9,"total_achievement_rate_percent_record","Decimal",false,"prim",true},
  {10,"best_record_challenge_count","int",false,"prim",false},
  {11,"best_record_date","DateTime",false,"prim",true},
  {12,"has_received_reward","bool",false,"prim",false}
};
static const FieldSpec _k_MusicCourseRankingPayload[] = {
  {0,"user_id","long",false,"prim",false},
  {1,"current_challenge_count","int",false,"prim",false},
  {2,"perfect_star","int",false,"prim",false},
  {3,"perfect","int",false,"prim",false},
  {4,"great","int",false,"prim",false},
  {5,"good","int",false,"prim",false},
  {6,"bad","int",false,"prim",false},
  {7,"miss","int",false,"prim",false},
  {8,"total_achievement_rate_percent_record","Decimal",false,"prim",true},
  {9,"best_record_challenge_count","int",false,"prim",false},
  {10,"best_record_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_MusicMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {4,"reward_rule_master_id","long",false,"prim",false},
  {5,"pronounce_name","string",false,"prim",true},
  {6,"lyric_writer","string",false,"prim",true},
  {7,"composer","string",false,"prim",true},
  {8,"arranger","string",false,"prim",true},
  {10,"unlock_text","string",false,"prim",true},
  {11,"is_long_version","bool",false,"prim",false},
  {12,"released_at","DateTime",false,"prim",true},
  {13,"stamina_consumption","int",false,"prim",false},
  {14,"music_time_second","int",false,"prim",false},
  {15,"invisible","bool",false,"prim",false},
  {17,"sample_start_seconds","float",false,"prim",false},
  {18,"sample_end_seconds","float",false,"prim",false},
  {19,"delay_seconds","float",false,"prim",false},
  {20,"vocal_versions","MusicVocalVersionMaster",true,"model",true},
  {21,"unlock_condition_type","MusicUnlockConditionTypes",false,"enum",false},
  {22,"unlock_condition_value","long",false,"prim",true},
  {23,"music_video_type","MusicVideoTypes",false,"enum",false},
  {24,"music_cover_type","MusicCoverTypes",false,"enum",false},
  {25,"story_event_master_id","long",false,"prim",true},
  {26,"story_master_id","long",false,"prim",true},
  {27,"event_master_id","long",false,"prim",true}
};
static const FieldSpec _k_MusicVideo[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_video_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MusicVocalVersionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"vocal_version","int",false,"prim",false},
  {3,"singer","string",false,"prim",true},
  {4,"name","string",false,"prim",true},
  {5,"music_time_second","int",false,"prim",false},
  {6,"sample_start_seconds","float",false,"prim",false},
  {7,"sample_end_seconds","float",false,"prim",false},
  {8,"music_video_type","MusicVideoTypes",false,"enum",false},
  {9,"characters","long",true,"prim",true}
};
static const FieldSpec _k_MyCircleInformationResult[] = {
  {0,"u_circle_hashed_id","string",false,"prim",true},
  {1,"name","string",false,"prim",true},
  {2,"comment","string",false,"prim",true},
  {3,"play_time_start_type","PlayTimeTypes",false,"enum",false},
  {4,"play_time_end_type","PlayTimeTypes",false,"enum",false},
  {5,"entry_type","EntryTypes",false,"enum",false},
  {6,"member_count","int",false,"prim",false},
  {7,"my_authority","CircleAuthorities",false,"enum",false},
  {8,"result_status","CircleResultStatus",false,"enum",false},
  {9,"circle_banner","CircleBanner",false,"model",true},
  {10,"company_master_id","long",false,"prim",true},
  {11,"character_base_master_id","long",false,"prim",true},
  {12,"support_company_information","SupportCompanyInformation",false,"model",true},
  {13,"support_company","Companies",false,"enum",false},
  {14,"daily_added_support_point","int",false,"prim",false},
  {15,"is_publish_ranking","bool",false,"prim",false},
  {16,"stamina_last_received_at","DateTime",false,"prim",false},
  {17,"circle_authority_update_type","CircleAuthorityUpdateTypes",false,"enum",false}
};
static const FieldSpec _k_NameBaseColor[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name_base_color_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_NameColor[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name_color_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_NameColorMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false},
  {6,"unlock_text","string",false,"prim",true}
};
static const FieldSpec _k_Nameplate[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name_plate_master_id","long",false,"prim",false}
};
static const FieldSpec _k_NameplateDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"class_type","LeagueClassTypes",false,"enum",false},
  {3,"enroll_count","int",false,"prim",false},
  {4,"change_value1","long",false,"prim",true},
  {5,"change_value2","long",false,"prim",true},
  {6,"priority","int",false,"prim",false}
};
static const FieldSpec _k_NameplateMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false},
  {6,"details","NameplateDetailMaster",true,"model",true},
  {7,"unlock_text","string",false,"prim",true},
  {8,"change_type","NamePlateChangeTypes",false,"enum",false},
  {9,"change_value1","long",false,"prim",true},
  {10,"change_value2","long",false,"prim",true}
};
static const FieldSpec _k_Note[] = {
  {0,"id_","long",false,"prim",false},
  {1,"note_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_NoteJudgePayload[] = {
  {0,"timing","TimingTypes",false,"enum",false},
  {1,"count","int",false,"prim",false}
};
static const FieldSpec _k_NoteMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false}
};
static const FieldSpec _k_NoteResult[] = {
  {0,"perfect_star","int",false,"prim",false},
  {1,"perfect","int",false,"prim",false},
  {2,"great","int",false,"prim",false},
  {3,"good","int",false,"prim",false},
  {4,"bad","int",false,"prim",false},
  {5,"miss","int",false,"prim",false}
};
static const FieldSpec _k_Notification[] = {
  {0,"id_","long",false,"prim",false},
  {1,"important_read_at","DateTime",false,"prim",false},
  {2,"update_read_at","DateTime",false,"prim",false},
  {3,"bug_read_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_NotificationContentResult[] = {
  {0,"id_","long",false,"prim",false},
  {1,"body","string",false,"prim",true},
  {2,"title","string",false,"prim",true},
  {3,"posting_at","DateTime",false,"prim",false},
  {4,"last_updated_at","DateTime",false,"prim",false},
  {5,"notification_tab_category","NotificationTabCategory",false,"enum",false},
  {6,"notification_category","NotificationCategory",false,"enum",false},
  {7,"banner_path","string",false,"prim",true}
};
static const FieldSpec _k_NotificationResult[] = {
  {0,"id_","long",false,"prim",false},
  {1,"banner_path","string",false,"prim",true},
  {2,"title","string",false,"prim",true},
  {3,"posting_at","DateTime",false,"prim",false},
  {4,"last_updated_at","DateTime",false,"prim",false},
  {5,"notification_tab_category","NotificationTabCategory",false,"enum",false},
  {6,"notification_category","NotificationCategory",false,"enum",false},
  {7,"is_confirmation","bool",false,"prim",false},
  {8,"order","int",false,"prim",false}
};
static const FieldSpec _k_Party[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_PartyInfo[] = {
  {0,"leader_position","int",false,"prim",false},
  {1,"multi_room_party_slots","MultiRoomPartySlot",true,"model",true}
};
static const FieldSpec _k_PartyPayload[] = {
  {0,"leader_position","int",false,"prim",false},
  {1,"party_slot_payload","PartySlotPayload",true,"model",true}
};
static const FieldSpec _k_PartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"character_id","long",false,"prim",false},
  {4,"poster_id","long",false,"prim",true},
  {5,"accessory_id","long",false,"prim",true},
  {6,"bonus_ability_enable_flags","BonusAbilityEnableFlags",false,"enum",false}
};
static const FieldSpec _k_PartySlotAccessoryPayload[] = {
  {0,"m_accessory_id","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"m_accessory_effect_id1","long",false,"prim",true},
  {3,"m_accessory_effect_id2","long",false,"prim",true},
  {4,"m_accessory_effect_id3","long",false,"prim",true}
};
static const FieldSpec _k_PartySlotCharacterPayload[] = {
  {0,"m_character_id","long",false,"prim",false},
  {1,"talent_stage","int",false,"prim",false},
  {2,"awakening_phase","int",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"sense_level","int",false,"prim",false},
  {5,"current_experience","int",false,"prim",false},
  {6,"released_episode_order","CharacterEpisodeOrder",false,"enum",false},
  {7,"read_episode_order","CharacterEpisodeOrder",false,"enum",false},
  {8,"display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_PartySlotDetail[] = {
  {0,"position","int",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"character_level","int",false,"prim",false},
  {3,"character_talent_stage","int",false,"prim",false},
  {4,"character_awakening_phase","int",false,"prim",false},
  {5,"character_display_awakening_status","bool",false,"prim",false},
  {6,"poster_master_id","long",false,"prim",true},
  {7,"poster_level","int",false,"prim",true},
  {8,"poster_breakthrough_phase","int",false,"prim",true},
  {9,"accessory_master_id","long",false,"prim",true},
  {10,"accessory_level","int",false,"prim",true},
  {11,"current_status_vocal","int",false,"prim",false},
  {12,"current_status_expression","int",false,"prim",false},
  {13,"current_status_concentration","int",false,"prim",false},
  {14,"u_accessory_id","long",false,"prim",true}
};
static const FieldSpec _k_PartySlotPayload[] = {
  {0,"position","int",false,"prim",false},
  {1,"party_slot_character_payload","PartySlotCharacterPayload",false,"model",true},
  {2,"party_slot_poster_payload","PartySlotPosterPayload",false,"model",true},
  {3,"party_slot_accessory_payload","PartySlotAccessoryPayload",false,"model",true}
};
static const FieldSpec _k_PartySlotPosterPayload[] = {
  {0,"m_poster_id","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"phase","int",false,"prim",false},
  {3,"released_episode","PosterEpisodeTypes",false,"enum",false}
};
static const FieldSpec _k_PermanentMarketThing[] = {
  {0,"permanent_market_thing_master_id","long",false,"prim",false},
  {1,"purchase_count","int",false,"prim",false}
};
static const FieldSpec _k_Photo[] = {
  {0,"id_","long",false,"prim",false},
  {1,"file_name","string",false,"prim",true},
  {2,"sas_token","string",false,"prim",true},
  {3,"photo_effect_master_id","long",false,"prim",true},
  {4,"lock","bool",false,"prim",false},
  {5,"use_album_page","int",false,"prim",true},
  {6,"level","int",false,"prim",false},
  {7,"rarity","PhotoRarities",false,"enum",false},
  {8,"sign_master_id","long",false,"prim",true},
  {9,"generated_at","DateTime",false,"prim",false},
  {10,"thumbnail_sas_token","string",false,"prim",true},
  {11,"appeared_character_base_master_ids","long",true,"prim",true},
  {12,"tagged_character_base_master_ids","long",true,"prim",true},
  {13,"use_deco_page","UseDecoPageFlag",false,"enum",false}
};
static const FieldSpec _k_PhotoAppearedCharacter[] = {
  {0,"character_base_master_id","long",false,"prim",false},
  {1,"is_main_character","bool",false,"prim",false}
};
static const FieldSpec _k_PickupCharacterMission[] = {
  {0,"pickup_character_mission_master_id","long",false,"prim",false},
  {1,"received_detail_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_PlayerRankPointResult[] = {
  {0,"rank_before","int",false,"prim",false},
  {1,"rank_after","int",false,"prim",false},
  {2,"rank_point_before","int",false,"prim",false},
  {3,"rank_point_after","int",false,"prim",false},
  {4,"rank_point_acquired","int",false,"prim",false},
  {5,"stamina_before","int",false,"prim",false}
};
static const FieldSpec _k_Poster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"poster_master_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"breakthrough_phase","int",false,"prim",false},
  {4,"released_episode","PosterEpisodeTypes",false,"enum",false},
  {5,"item_consume_break_through_count","int",false,"prim",false},
  {6,"is_favorite","bool",false,"prim",false},
  {7,"alternative_image_pattern","int",false,"prim",false}
};
static const FieldSpec _k_PosterAlternativeImagePayload[] = {
  {0,"poster_id","long",false,"prim",false},
  {1,"pattern","int",false,"prim",false}
};
static const FieldSpec _k_PosterCostumeMaster[] = {
  {0,"phase","int",false,"prim",false},
  {1,"costume_master_id","long",false,"prim",true},
  {2,"item_master_id","long",false,"prim",true},
  {3,"quantity","int",false,"prim",false},
  {4,"decoration_master_id","long",false,"prim",true}
};
static const FieldSpec _k_PosterFavoritePayload[] = {
  {0,"poster_id","long",false,"prim",false},
  {1,"set_favorite","bool",false,"prim",false}
};
static const FieldSpec _k_PosterLevelPatternGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"patterns","PosterLevelPatternMaster",true,"model",true}
};
static const FieldSpec _k_PosterLevelPatternMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"level_pattern_group_id","long",false,"prim",false},
  {2,"level","int",false,"prim",false},
  {3,"item_master_id","long",false,"prim",false},
  {4,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_PosterLineupResult[] = {
  {0,"normal_probabilities","PosterRarityProbability",true,"model",true},
  {1,"fixed_probabilities","PosterRarityProbability",true,"model",true},
  {2,"normal_lineup_items","GachaLineupItemProbability",true,"model",true},
  {3,"fixed_lineup_items","GachaLineupItemProbability",true,"model",true}
};
static const FieldSpec _k_PosterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"organize_restrict_group_id","int",false,"prim",true},
  {3,"rarity","PossessionRarities",false,"enum",false},
  {5,"level_pattern_group_master_id","long",false,"prim",false},
  {6,"sub_title_position_x1","float",false,"prim",true},
  {7,"sub_title_position_y1","float",false,"prim",true},
  {8,"sub_title_position_x2","float",false,"prim",true},
  {9,"sub_title_position_y2","float",false,"prim",true},
  {10,"sub_title_position_x3","float",false,"prim",true},
  {11,"sub_title_position_y3","float",false,"prim",true},
  {12,"release_item_group_id","long",false,"prim",false},
  {15,"pronounce_name","string",false,"prim",true},
  {16,"costumes","PosterCostumeMaster",true,"model",true},
  {17,"appearance_character_base_master_ids","long",true,"prim",true},
  {18,"is_restrict_item_break_through","bool",false,"prim",false},
  {19,"display_start_at","DateTime",false,"prim",false},
  {20,"display_end_at","DateTime",false,"prim",false},
  {21,"unlock_text","string",false,"prim",true},
  {22,"orientation","PosterOrientation",false,"enum",false},
  {23,"sub_title_display_condition","PosterSubTitleDisplayConditions",false,"enum",false},
  {24,"sub_title_display_condition_value","int",false,"prim",true},
  {25,"poster_breakthrough_max_phase","int",false,"prim",true},
  {26,"poster_breakthrough_max_phase_release_date","DateTime",false,"prim",true},
  {27,"secondary_sub_title_display_condition","PosterSubTitleDisplayConditions",false,"enum",false},
  {28,"secondary_sub_title_display_condition_value","int",false,"prim",true},
  {29,"alternate_image_position_x1","float",false,"prim",true},
  {30,"alternate_image_position_y1","float",false,"prim",true},
  {31,"alternate_image_release_phase1","int",false,"prim",true},
  {32,"alternate_image_position_x2","float",false,"prim",true},
  {33,"alternate_image_position_y2","float",false,"prim",true},
  {34,"alternate_image_release_phase2","int",false,"prim",true},
  {35,"alternate_image_position_x3","float",false,"prim",true},
  {36,"alternate_image_position_y3","float",false,"prim",true},
  {37,"alternate_image_release_phase3","int",false,"prim",true}
};
static const FieldSpec _k_PosterRarityProbability[] = {
  {0,"rarity","PossessionRarities",false,"enum",false},
  {1,"probability","double",false,"prim",false}
};
static const FieldSpec _k_PosterReleaseItemGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"items","PosterReleaseItemMaster",true,"model",true},
  {2,"item_consume_apply_flag","bool",false,"prim",false}
};
static const FieldSpec _k_PosterReleaseItemMaster[] = {
  {0,"current_phase","int",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PosterStoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"poster_master_id","long",false,"prim",false},
  {2,"episode_type","PosterEpisodeTypes",false,"enum",false},
  {3,"character_base_master_id","long",false,"prim",true},
  {4,"description","string",false,"prim",true},
  {5,"order","int",false,"prim",false},
  {6,"character_icon_id","long",false,"prim",true},
  {7,"character_name","string",false,"prim",true}
};
static const FieldSpec _k_ProcessPaymentResult[] = {
  {0,"result","ProcessPaymentTransactionResult",false,"enum",false}
};
static const FieldSpec _k_PurchaseItemPayload[] = {
  {0,"m_jewel_shop_item_id","long",false,"prim",false}
};
static const FieldSpec _k_RandomEffectGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"accessory_effects","long",true,"prim",true}
};
static const FieldSpec _k_RateResult[] = {
  {0,"achievement_rate_result","RateUpdateResult",false,"model",true},
  {1,"live_rate_result","RateUpdateResult",false,"model",true},
  {2,"total_rate_before","double",false,"prim",false},
  {3,"total_rate_after","double",false,"prim",false}
};
static const FieldSpec _k_RateUpdateResult[] = {
  {0,"best_ever","double",false,"prim",false},
  {1,"this_time","double",false,"prim",false}
};
static const FieldSpec _k_RawHighScoreRankingResult[] = {
  {0,"raw_ranking","RawRankingWithLongPoint",true,"model",true},
  {1,"high_score_parties","HighScoreParty",true,"model",true},
  {2,"high_score_buffs","HighScoreBuff",true,"model",true},
  {3,"user_profiles","UserProfile",true,"model",true}
};
static const FieldSpec _k_RawRanking[] = {
  {0,"rank","int",false,"prim",false},
  {1,"point","int",false,"prim",false},
  {2,"user_id","string",false,"prim",true},
  {3,"group_value","int",false,"prim",false}
};
static const FieldSpec _k_RawRankingResult[] = {
  {0,"raw_ranking","RawRanking",true,"model",true},
  {1,"user_profiles","UserProfile",true,"model",true}
};
static const FieldSpec _k_RawRankingWithLongPoint[] = {
  {0,"rank","int",false,"prim",false},
  {1,"point","long",false,"prim",false},
  {2,"user_id","string",false,"prim",true}
};
static const FieldSpec _k_ReadNotificationPayload[] = {
  {0,"tab_category","NotificationTabCategory",false,"enum",false},
  {1,"read_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_ReceivedThing[] = {
  {0,"type","ThingTypes",false,"enum",false},
  {1,"id_","long",false,"prim",true},
  {2,"quantity","int",false,"prim",false},
  {3,"original_type","ThingTypes",false,"enum",true},
  {4,"original_id","long",false,"prim",true},
  {5,"after_phase","int",false,"prim",true},
  {6,"sent_inbox","bool",false,"prim",false}
};
static const FieldSpec _k_RecyclableMonoBehaviour[] = {
};
static const FieldSpec _k_RegisterAppStorePaymentPayload[] = {
  {0,"receipt_base64","string",false,"prim",true}
};
static const FieldSpec _k_RegisterBirthDayPayload[] = {
  {0,"birth_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_RegisterGooglePlayPaymentPayload[] = {
  {0,"json_","string",false,"prim",true}
};
static const FieldSpec _k_RegisterPayload[] = {
  {0,"name","string",false,"prim",true}
};
static const FieldSpec _k_RegisterTakeOverPasswordPayload[] = {
  {0,"password","string",false,"prim",true}
};
static const FieldSpec _k_Restriction[] = {
  {0,"id_","long",false,"prim",false},
  {1,"multi_live_restriction_finished_at","DateTime",false,"prim",true},
  {2,"read_multi_live_restriction_dialog","bool",false,"prim",true}
};
static const FieldSpec _k_RewardRuleMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"achivement_rate_rewards","AchivementRateRewardMaster",true,"model",true}
};
static const FieldSpec _k_RollResult[] = {
  {0,"roll_order","int",false,"prim",false},
  {1,"selected_slot","int",false,"prim",false},
  {2,"roulette_prize_master_id","long",false,"prim",false}
};
static const FieldSpec _k_Roulette[] = {
  {0,"id_","long",false,"prim",false},
  {1,"roulette_master_id","long",false,"prim",false},
  {2,"roll_count","int",false,"prim",false}
};
static const FieldSpec _k_RouletteEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"roulette_event_master_id","long",false,"prim",false},
  {2,"total_acquired_point","int",false,"prim",false}
};
static const FieldSpec _k_RouletteRollResult[] = {
  {0,"roll_results","RollResult",true,"model",true}
};
static const FieldSpec _k_ScoreWithDateResult[] = {
  {0,"score","long",false,"prim",false},
  {1,"date","DateTime",false,"prim",false}
};
static const FieldSpec _k_SelectMusicCourseRandomMusicPayload[] = {
  {0,"music_course_master_id","long",false,"prim",false},
  {1,"music_course_gauge_type","MusicCourseGaugeType",false,"enum",false}
};
static const FieldSpec _k_SellAccessoryPayload[] = {
  {0,"u_accessory_ids","long",true,"prim",true}
};
static const FieldSpec _k_Sense[] = {
  {0,"id_","long",false,"prim",false},
  {1,"actor_id","long",false,"prim",false},
  {3,"cool_time","int",false,"prim",false},
  {4,"sense_type","SenseTypes",false,"enum",false},
  {5,"acquirable_lights","SenseLightTypes",true,"enum",true},
  {6,"pre_sense_effect","Effect",true,"model",true},
  {7,"sense_effect","SenseEffect",false,"model",true},
  {8,"poster_effect","Effect",true,"model",true},
  {9,"accessory_effect","Effect",true,"model",true},
  {10,"combination_sense_id","long",true,"prim",true},
  {11,"sense_master_id","long",false,"prim",false},
  {12,"original_actor_id","long",false,"prim",false}
};
static const FieldSpec _k_SenseCoolTime[] = {
  {0,"position","int",false,"prim",false},
  {1,"cool_time","int",false,"prim",false}
};
static const FieldSpec _k_SenseEffect[] = {
  {0,"score_factor","int",false,"prim",false},
  {1,"principal","int",false,"prim",false},
  {2,"branch_condition","BranchConditionTypes",false,"enum",false},
  {3,"effect_branches","EffectBranch",true,"model",true}
};
static const FieldSpec _k_SenseEffectMaster[] = {
  {0,"order","int",false,"prim",false},
  {1,"effect_master_id","long",false,"prim",false}
};
static const FieldSpec _k_SenseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"type","SenseTypes",false,"enum",false},
  {4,"pre_effects","EffectOrderMaster",true,"model",true},
  {5,"branches","BranchMaster",true,"model",true},
  {6,"acquirable_gauge","int",false,"prim",false},
  {7,"acquirable_score_percent","int",false,"prim",false},
  {8,"score_up_per_level","int",false,"prim",false},
  {9,"light_count","int",false,"prim",false},
  {10,"cool_time","int",false,"prim",false},
  {11,"branch_condition1","BranchConditionType",false,"enum",false},
  {12,"condition_value1","long",false,"prim",true},
  {13,"branch_condition2","BranchConditionType",false,"enum",false},
  {14,"condition_value2","long",false,"prim",true},
  {15,"sub_types","SenseTypes",true,"enum",true}
};
static const FieldSpec _k_SenseScoreBlock[] = {
  {0,"hash","long",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"life","int",false,"prim",false},
  {3,"time_event_second","int",false,"prim",false},
  {4,"sense_id","long",false,"prim",false},
  {5,"combo","int",false,"prim",false}
};
static const FieldSpec _k_SenseTimingEvent[] = {
  {0,"timing_seconds","int",false,"prim",false},
  {1,"position","int",false,"prim",false},
  {2,"event","TimingEvent",false,"model",true}
};
static const FieldSpec _k_SetAlbumPublishingPayload[] = {
  {0,"publishing","bool",false,"prim",false},
  {1,"publish_page_number","int",false,"prim",false}
};
static const FieldSpec _k_SetLessonPartyPayload[] = {
  {0,"slots","SetLessonPartySlotPayload",true,"model",true}
};
static const FieldSpec _k_SetLessonPartySlotPayload[] = {
  {0,"order","int",false,"prim",false},
  {1,"character_id","long",false,"prim",false}
};
static const FieldSpec _k_SetPhotoTagPayload[] = {
  {0,"photo_id","long",false,"prim",false},
  {1,"character_base_ids","long",true,"prim",true}
};
static const FieldSpec _k_SetSelectedThingsPayload[] = {
  {0,"gacha_thing_ids","long",true,"prim",true}
};
static const FieldSpec _k_SpRate[] = {
  {0,"id_","long",false,"prim",false},
  {1,"live_master_id","long",false,"prim",false},
  {2,"point","int",false,"prim",false}
};
static const FieldSpec _k_SpRateUpdateResult[] = {
  {0,"best_ever","int",false,"prim",false},
  {1,"this_time","int",false,"prim",false},
  {2,"best_ever_total","int",false,"prim",false},
  {3,"this_time_total","int",false,"prim",false}
};
static const FieldSpec _k_SpecialEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"special_event_master_id","long",false,"prim",false},
  {2,"read_tips","bool",false,"prim",false}
};
static const FieldSpec _k_SpotConversationMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"spot","SpotTypes",false,"enum",false},
  {2,"character_id1","long",false,"prim",true},
  {3,"character_id2","long",false,"prim",true},
  {4,"character_id3","long",false,"prim",true},
  {5,"character_id4","long",false,"prim",true},
  {8,"character_id5","long",false,"prim",true},
  {9,"episode_master_id","long",false,"prim",false},
  {10,"costume_id1","long",false,"prim",true},
  {11,"costume_id2","long",false,"prim",true},
  {12,"costume_id3","long",false,"prim",true},
  {13,"costume_id4","long",false,"prim",true},
  {14,"costume_id5","long",false,"prim",true},
  {15,"title","string",false,"prim",true}
};
static const FieldSpec _k_Stamp[] = {
  {0,"id_","long",false,"prim",false},
  {1,"stamp_master_ids","long",true,"prim",true},
  {2,"favorite_stamp_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_StampMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"is_default","bool",false,"prim",false},
  {3,"character_base_master_id","long",false,"prim",false},
  {4,"type","StampType",false,"enum",false},
  {5,"asset_id","string",false,"prim",true},
  {6,"voice_asset_id","string",false,"prim",true},
  {7,"name","string",false,"prim",true},
  {8,"character_base_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_StarAct[] = {
  {0,"score_factor","int",false,"prim",false},
  {1,"actor_id","long",false,"prim",false},
  {2,"branch_condition","BranchConditionTypes",false,"enum",false},
  {3,"effect_branches","EffectBranch",true,"model",true}
};
static const FieldSpec _k_StarActScoreBlock[] = {
  {0,"hash","long",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"life","int",false,"prim",false},
  {3,"time_event_second","int",false,"prim",false},
  {4,"combo","int",false,"prim",false}
};
static const FieldSpec _k_StarPassStatus[] = {
  {0,"id_","long",false,"prim",false},
  {1,"type","StarPassTypes",false,"enum",false},
  {2,"total_purchased_count","int",false,"prim",false},
  {3,"valid_until","DateTime",false,"prim",false}
};
static const FieldSpec _k_StarPointResult[] = {
  {0,"rank_before","int",false,"prim",false},
  {1,"rank_after","int",false,"prim",false},
  {2,"star_point_before","int",false,"prim",false},
  {3,"star_point_after","int",false,"prim",false},
  {4,"star_point_acquired","int",false,"prim",false},
  {5,"received_reward","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_StarRankRewardMaster[] = {
  {0,"rank","int",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"character_star_rank_reward_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StartLessonPayload[] = {
  {0,"character_base_master_id","int",false,"prim",false},
  {1,"live_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StartLivePayload[] = {
  {0,"party_id","long",false,"prim",false},
  {1,"live_master_id","long",false,"prim",false},
  {2,"audition_master_id","long",false,"prim",true},
  {3,"league_master_id","long",false,"prim",true},
  {4,"use_stamina","bool",false,"prim",false},
  {5,"stamina_consumption_ratio","int",false,"prim",false},
  {6,"live_setting_master_id","long",false,"prim",false},
  {7,"is_auto_play","bool",false,"prim",false},
  {8,"is_story_event_challenge","bool",false,"prim",false},
  {9,"concert_stage_master_id","long",false,"prim",true},
  {10,"bonus_live_stage_master_id","long",false,"prim",true},
  {11,"music_course_detail_master_id","long",false,"prim",true},
  {12,"music_course_gauge_type","MusicCourseGaugeType",false,"enum",false},
  {13,"ghost_live_master_id","long",false,"prim",true},
  {14,"trial_party_event_stage_master_id","long",false,"prim",true}
};
static const FieldSpec _k_StartMultiLivePayload[] = {
  {0,"live_master_id","long",false,"prim",false},
  {1,"multi_live_id","long",false,"prim",false},
  {2,"use_stamina","bool",false,"prim",false},
  {3,"stamina_consumption_ratio","int",false,"prim",false},
  {4,"party_id","long",false,"prim",true}
};
static const FieldSpec _k_StartMultiRoomLivePayload[] = {
  {0,"hashed_multi_room_id","string",false,"prim",true},
  {1,"live_master_id","long",false,"prim",false},
  {2,"party_id","long",false,"prim",true}
};
static const FieldSpec _k_StartTournamentPayload[] = {
  {0,"tournament_detail_master_id","long",false,"prim",false},
  {1,"party_id","long",false,"prim",false},
  {2,"stamina_consumption_ratio","int",false,"prim",false}
};
static const FieldSpec _k_StartTripleCastLivePayload[] = {
  {0,"m_live_id","long",false,"prim",false},
  {1,"m_triple_cast_id","long",false,"prim",false}
};
static const FieldSpec _k_StartTripleCastLiveResult[] = {
  {0,"live_units","LiveUnitWithOrder",true,"model",true}
};
static const FieldSpec _k_Status[] = {
  {0,"vocal","int",false,"prim",false},
  {1,"expression","int",false,"prim",false},
  {2,"concentration","int",false,"prim",false}
};
static const FieldSpec _k_StoryEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"total_acquired_point","int",false,"prim",false},
  {3,"acquired_point_updated_date","DateTime",false,"prim",false},
  {4,"last_rank","int",false,"prim",true},
  {5,"read_tips","bool",false,"prim",false},
  {6,"login_days","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventCampInfo[] = {
  {0,"raw_ranking","RawRanking",false,"model",true},
  {1,"camp1_total_point","long",false,"prim",false},
  {2,"camp2_total_point","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircle[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"current_point","int",false,"prim",false},
  {3,"high_score","long",false,"prim",false},
  {4,"circle_id","long",false,"prim",true}
};
static const FieldSpec _k_StoryEventCircleMission[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_circle_mission_master_id","long",false,"prim",false},
  {2,"current_count","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircleMissionReward[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"story_event_circle_mission_reward_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScore[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"current_enhancement_point","int",false,"prim",false},
  {3,"total_acquired_enhancement_point","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScoreBuffSetting[] = {
  {0,"story_event_high_score_buff_setting_master_id","long",false,"prim",false},
  {1,"current_level","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScoreParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {3,"high_score","long",false,"prim",false},
  {5,"rate_grade","AchievementRateGrades",false,"enum",false},
  {6,"difficulty","MusicDifficulties",false,"enum",false},
  {8,"high_score_type","HighScoreTypes",false,"enum",false},
  {9,"live_setting_master_id","long",false,"prim",false},
  {10,"slots","StoryEventHighScorePartySlot",true,"model",true},
  {11,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScorePartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_high_score_clear_party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"character_master_id","long",false,"prim",false},
  {4,"character_level","int",false,"prim",false},
  {5,"character_talent_stage","int",false,"prim",false},
  {6,"character_awakening_phase","int",false,"prim",false},
  {7,"poster_master_id","long",false,"prim",true},
  {8,"poster_level","int",false,"prim",true},
  {9,"poster_breakthrough_phase","int",false,"prim",true},
  {10,"accessory_master_id","long",false,"prim",true},
  {11,"accessory_level","int",false,"prim",true},
  {12,"current_status","Status",false,"model",true},
  {13,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_StoryEventMissionCircleProgress[] = {
  {0,"story_event_circle_mission_master_id","long",false,"prim",false},
  {1,"current_count","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventMissionCircleProgressResult[] = {
  {0,"is_success","bool",false,"prim",false},
  {1,"progresses","StoryEventMissionCircleProgress",true,"model",true}
};
static const FieldSpec _k_StoryEventPointExchangeResult[] = {
  {0,"story_event_master_id","long",false,"prim",false},
  {1,"before_story_event_point_amount","int",false,"prim",false},
  {2,"after_coin_amount","int",false,"prim",false}
};
static const FieldSpec _k_StoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"type","StoryTypes",false,"enum",false},
  {2,"company_master_id","long",false,"prim",true},
  {3,"event_master_id","long",false,"prim",true},
  {4,"chapter_order","int",false,"prim",false},
  {5,"display_start_at","DateTime",false,"prim",false},
  {6,"display_end_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_SupportCompanyInformation[] = {
  {0,"sirius","SupportCompanyLevelStatus",false,"model",true},
  {1,"eden","SupportCompanyLevelStatus",false,"model",true},
  {2,"gingaza","SupportCompanyLevelStatus",false,"model",true},
  {3,"denki","SupportCompanyLevelStatus",false,"model",true}
};
static const FieldSpec _k_SupportCompanyLevelLimitDetail[] = {
  {0,"order","int",false,"prim",false},
  {1,"quantity","long",false,"prim",false}
};
static const FieldSpec _k_SupportCompanyLevelLimitPayload[] = {
  {0,"company","Companies",false,"enum",false},
  {1,"coin_quantity","long",false,"prim",false},
  {2,"details","SupportCompanyLevelLimitDetail",true,"model",true}
};
static const FieldSpec _k_SupportCompanyLevelLimitStatus[] = {
  {0,"current_coin_quantity","long",false,"prim",false},
  {1,"details","SupportCompanyLevelLimitStatusDetail",true,"model",true}
};
static const FieldSpec _k_SupportCompanyLevelLimitStatusDetail[] = {
  {0,"order","int",false,"prim",false},
  {1,"current_quantity","long",false,"prim",false}
};
static const FieldSpec _k_SupportCompanyLevelStatus[] = {
  {0,"level","int",false,"prim",false},
  {1,"current_support_point","int",false,"prim",false},
  {2,"last_level_upped_at","DateTime",false,"prim",false},
  {3,"level_limit","int",false,"prim",false},
  {4,"level_limit_status","SupportCompanyLevelLimitStatus",false,"model",true}
};
static const FieldSpec _k_TakeOverAccountPayload[] = {
  {0,"linkage_code","string",false,"prim",true},
  {1,"password","string",false,"prim",true}
};
static const FieldSpec _k_TakeOverAccountResult[] = {
  {0,"is_success","bool",false,"prim",false},
  {1,"user_id","string",false,"prim",true},
  {2,"name","string",false,"prim",true},
  {3,"rank","int",false,"prim",false},
  {4,"login_token","string",false,"prim",true}
};
static const FieldSpec _k_TakeOverCodeResult[] = {
  {0,"is_success","bool",false,"prim",false},
  {1,"linkage_code","string",false,"prim",true}
};
static const FieldSpec _k_TheaterStory[] = {
  {0,"id_","long",false,"prim",false},
  {1,"theater_story_master_id","long",false,"prim",false}
};
static const FieldSpec _k_TimeLimitedControl[] = {
  {0,"id_","long",false,"prim",false},
  {1,"time_limited_control_master_id","long",false,"prim",false},
  {2,"expired_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_TimedConfirmationCode[] = {
  {0,"confirmation_code","string",false,"prim",true},
  {1,"remaining_seconds","int",false,"prim",false}
};
static const FieldSpec _k_TimingEvent[] = {
  {0,"total_sense_lights","SenseLightTypes",true,"enum",true},
  {1,"grant_sense_lights","SenseLightTypes",true,"enum",true},
  {2,"is_star_act","bool",false,"prim",false},
  {3,"lost_lights","bool",false,"prim",false},
  {4,"acquirable_lights","SenseLightTypes",true,"enum",true},
  {5,"sense_ids","long",true,"prim",true},
  {6,"sense_voice_ids","long",true,"prim",true},
  {7,"sense_master_id","long",false,"prim",false}
};
static const FieldSpec _k_TotalPointEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"total_point_event_master_id","long",false,"prim",false},
  {2,"total_acquired_point","long",false,"prim",false},
  {3,"received_reward_order","int",false,"prim",false}
};
static const FieldSpec _k_TotalPointEventInformationResult[] = {
  {0,"current_total_point","long",false,"prim",false}
};
static const FieldSpec _k_TotalPointEventRankingResult[] = {
  {0,"raw_ranking","RawRankingWithLongPoint",true,"model",true},
  {1,"user_profiles","UserProfile",true,"model",true}
};
static const FieldSpec _k_TournamentDetail[] = {
  {0,"id_","long",false,"prim",false},
  {1,"tournament_detail_master_id","long",false,"prim",false},
  {2,"best_unique_score","long",false,"prim",false},
  {3,"perfect_star","int",false,"prim",false},
  {4,"perfect","int",false,"prim",false},
  {5,"great","int",false,"prim",false},
  {6,"good","int",false,"prim",false},
  {7,"bad","int",false,"prim",false},
  {8,"miss","int",false,"prim",false},
  {9,"recorded_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_TournamentQualifying[] = {
  {0,"id_","long",false,"prim",false},
  {1,"tournament_qualifying_master_id","long",false,"prim",false},
  {2,"music_course_master_id","long",false,"prim",false},
  {3,"current_challenge_count","int",false,"prim",false},
  {4,"perfect_star","int",false,"prim",false},
  {5,"perfect","int",false,"prim",false},
  {6,"great","int",false,"prim",false},
  {7,"good","int",false,"prim",false},
  {8,"bad","int",false,"prim",false},
  {9,"miss","int",false,"prim",false},
  {10,"total_achievement_rate_percent_record","Decimal",false,"prim",true},
  {11,"best_record_challenge_count","int",false,"prim",false},
  {12,"best_record_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_TournamentQualifyingInformationResult[] = {
  {0,"entry_code","string",false,"prim",true}
};
static const FieldSpec _k_TournamentResult[] = {
  {0,"best_ever_unique_score","long",false,"prim",false},
  {1,"this_time_unique_score","long",false,"prim",false},
  {2,"tournament_detail_master_id","long",false,"prim",false}
};
static const FieldSpec _k_TransitionTokenResult[] = {
  {0,"token","string",false,"prim",true}
};
static const FieldSpec _k_TrialPartyEvent[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_event_master_id","long",false,"prim",false},
  {2,"current_stage_order","int",false,"prim",false},
  {3,"is_completed","bool",false,"prim",false}
};
static const FieldSpec _k_TrialPartyEventResult[] = {
  {0,"rewards","ReceivedThing",true,"model",true}
};
static const FieldSpec _k_TrialPartyEventStage[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_event_stage_master_id","long",false,"prim",false},
  {2,"is_cleared","bool",false,"prim",false}
};
static const FieldSpec _k_TrialPartyEventStageParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_event_stage_master_id","long",false,"prim",false},
  {2,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_TrialPartyEventStagePartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_event_stage_party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"trial_party_character_master_id","long",false,"prim",false},
  {4,"trial_party_poster_master_id","long",false,"prim",true},
  {5,"trial_party_accessory_master_id","long",false,"prim",true}
};
static const FieldSpec _k_TrialPartyEventStageResult[] = {
  {0,"principal_gauge_value","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastBasic[] = {
  {0,"id_","long",false,"prim",false},
  {1,"star_enroll_count","int",false,"prim",false},
  {2,"dai_star_enroll_count","int",false,"prim",false},
  {3,"current_class_type","LeagueClassTypes",false,"enum",false},
  {4,"best_class_type","LeagueClassTypes",false,"enum",false},
  {5,"last_joined_triple_cast_season_master_id","long",false,"prim",true},
  {6,"party_order1","int",false,"prim",true},
  {7,"party_order2","int",false,"prim",true},
  {8,"party_order3","int",false,"prim",true}
};
static const FieldSpec _k_TripleCastGroup[] = {
  {0,"triple_cast_master_id","long",false,"prim",false},
  {1,"class_type","LeagueClassTypes",false,"enum",false},
  {2,"class_order","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastGroupMember[] = {
  {0,"triple_cast_group_id","long",false,"prim",false},
  {1,"triple_cast_master_id","long",false,"prim",false},
  {2,"best_score","long",false,"prim",true}
};
static const FieldSpec _k_TripleCastHighScoreParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"triple_cast_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"high_score","long",false,"prim",false},
  {4,"slots","TripleCastHighScorePartySlot",true,"model",true},
  {5,"acting_ability","int",false,"prim",false},
  {6,"leader_position","int",false,"prim",false},
  {7,"difficulty","MusicDifficulties",false,"enum",false}
};
static const FieldSpec _k_TripleCastHighScorePartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"triple_cast_high_score_party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"character_master_id","long",false,"prim",false},
  {4,"character_level","int",false,"prim",false},
  {7,"poster_master_id","long",false,"prim",true},
  {8,"poster_level","int",false,"prim",true},
  {9,"poster_breakthrough_phase","int",false,"prim",true},
  {10,"accessory_master_id","long",false,"prim",true},
  {11,"accessory_level","int",false,"prim",true},
  {12,"current_status","Status",false,"model",true},
  {13,"character_talent_stage","int",false,"prim",false},
  {14,"character_awakening_phase","int",false,"prim",false},
  {15,"character_display_awakening_status","bool",false,"prim",false}
};
static const FieldSpec _k_TripleCastHistory[] = {
  {0,"triple_cast_master_id","long",false,"prim",false},
  {1,"class_type","LeagueClassTypes",false,"enum",false},
  {2,"history_count","int",false,"prim",false},
  {3,"is_sended_reward","bool",false,"prim",false},
  {5,"is_played","bool",false,"prim",false},
  {6,"class_change_type","LeagueClassChangeTypes",false,"enum",false},
  {8,"group_rank","int",false,"prim",false},
  {9,"global_rank","int",false,"prim",false},
  {10,"all_class_global_rank","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastParty[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"leader_position","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastPartyAndRankingResult[] = {
  {0,"rank","int",false,"prim",true},
  {1,"user_id","string",false,"prim",true},
  {2,"best_score","long",false,"prim",false},
  {3,"triple_cast_master_id","long",false,"prim",false},
  {4,"class_type","LeagueClassTypes",false,"enum",false},
  {5,"difficulty","MusicDifficulties",false,"enum",false},
  {6,"user_name","string",false,"prim",true},
  {7,"parties","TripleCastPartyResult",true,"model",true}
};
static const FieldSpec _k_TripleCastPartyResult[] = {
  {0,"order","int",false,"prim",false},
  {1,"score","long",false,"prim",false},
  {2,"slots","LeaguePartySlotResult",true,"model",true}
};
static const FieldSpec _k_TripleCastPartyScore[] = {
  {0,"order","int",false,"prim",false},
  {1,"score","long",false,"prim",false}
};
static const FieldSpec _k_TripleCastPartySlot[] = {
  {0,"id_","long",false,"prim",false},
  {1,"party_id","long",false,"prim",false},
  {2,"position","int",false,"prim",false},
  {3,"character_id","long",false,"prim",false},
  {4,"poster_id","long",false,"prim",true},
  {5,"accessory_id","long",false,"prim",true},
  {6,"bonus_ability_enable_flags","BonusAbilityEnableFlags",false,"enum",false}
};
static const FieldSpec _k_TripleCastSeasonResult[] = {
  {0,"triple_cast_season_master_id","long",false,"prim",false},
  {1,"dai_star_max_enroll_count","int",false,"prim",false}
};
static const FieldSpec _k_Trophy[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trophy_master_id","long",false,"prim",false},
  {2,"trophy_group_master_id","long",false,"prim",false},
  {3,"current_order","int",false,"prim",false}
};
static const FieldSpec _k_TrophyGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"category","TrophyCategories",false,"enum",false}
};
static const FieldSpec _k_TrophyMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"rarity","PossessionRarities",false,"enum",false},
  {4,"order","int",false,"prim",false},
  {5,"trophy_group_master_id","long",false,"prim",false},
  {6,"hidden","bool",false,"prim",false},
  {7,"unlock_text","string",false,"prim",true}
};
static const FieldSpec _k_UpdateClearLampResult[] = {
  {0,"clear_lamp","ClearLamps",false,"enum",false}
};
static const FieldSpec _k_UpdateGameHintPayload[] = {
  {0,"categories","PageCategories",true,"enum",true}
};
static const FieldSpec _k_UpdateHomeDisplayPreferencePayload[] = {
  {0,"home_character_base_master_id","long",false,"prim",true},
  {2,"member_character_base_master_id","long",false,"prim",true},
  {3,"story_character_base_master_id","long",false,"prim",true},
  {4,"shop_character_base_master_id","long",false,"prim",true},
  {5,"home_costume_master_id","long",false,"prim",true},
  {7,"member_costume_master_id","long",false,"prim",true},
  {8,"story_costume_master_id","long",false,"prim",true},
  {9,"shop_costume_master_id","long",false,"prim",true},
  {10,"illust_character_master_id","long",false,"prim",false},
  {11,"display_awakening_status","bool",false,"prim",false},
  {12,"home_character_display_type","HomeCharacterDisplayTypes",false,"enum",false},
  {13,"login_bonus_character_base_master_id","long",false,"prim",true},
  {14,"login_bonus_spine_costume_master_id","long",false,"prim",true}
};
static const FieldSpec _k_UpdateLastViewedAtPayload[] = {
  {0,"viewed_shop_category_types","ViewedShopCategoryTypes",false,"enum",false},
  {1,"exchange_shop_master_id","long",false,"prim",true}
};
static const FieldSpec _k_UpdateTutorialPayload[] = {
  {0,"tutorial_status","TutorialStatus",false,"enum",false}
};
static const FieldSpec _k_UrlResult[] = {
  {0,"url","string",false,"prim",true}
};
static const FieldSpec _k_UseExperienceItemsPayload[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_UseStaminaRecoveryItem[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_UseStaminaRecoveryItemsPayload[] = {
  {0,"items","UseStaminaRecoveryItem",true,"model",true}
};
static const FieldSpec _k_User[] = {
  {0,"id_","long",false,"prim",false},
  {1,"player_rank","int",false,"prim",false},
  {2,"current_rank_point","int",false,"prim",false},
  {3,"current_stamina","int",false,"prim",false},
  {4,"max_stamina_restored_at","DateTime",false,"prim",false},
  {5,"paid_jewel","int",false,"prim",false},
  {6,"free_jewel","int",false,"prim",false},
  {7,"coin","int",false,"prim",false},
  {8,"player_rank_limit","int",false,"prim",false},
  {9,"stamina_recover_times_with_jewel","int",false,"prim",false},
  {10,"circle_usage_restrictions_end_time","DateTime",false,"prim",false},
  {11,"circle_id","string",false,"prim",true},
  {12,"game_start_at","DateTime",false,"prim",false},
  {13,"hash_user_id","string",false,"prim",true},
  {14,"ban_level","BanLevels",false,"enum",false},
  {15,"tutorial_status","TutorialStatus",false,"enum",false},
  {16,"monthly_payment","int",false,"prim",false},
  {17,"splash_last_displayed_at","DateTime",false,"prim",false},
  {18,"is_caped_player_rank","bool",false,"prim",false},
  {19,"require_caped_player_rank_announce","bool",false,"prim",false}
};
static const FieldSpec _k_UserBlock[] = {
  {0,"id_","long",false,"prim",false},
  {1,"block_user_id","string",false,"prim",true}
};
static const FieldSpec _k_UserBonus[] = {
  {0,"id_","long",false,"prim",false},
  {1,"experience_bonus","float",false,"prim",false},
  {2,"lesson_star_rank_bonus","float",false,"prim",false}
};
static const FieldSpec _k_UserPreference[] = {
  {0,"id_","long",false,"prim",false},
  {1,"multi_party_id","long",false,"prim",false},
  {2,"birth_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_UserProfile[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"introduction","string",false,"prim",true},
  {3,"main_u_character_id","long",false,"prim",false},
  {4,"m_nameplate_id","long",false,"prim",true},
  {5,"m_name_color_id","long",false,"prim",false},
  {6,"m_trophy_id1","long",false,"prim",true},
  {7,"m_trophy_id2","long",false,"prim",true},
  {8,"m_trophy_id3","long",false,"prim",true},
  {9,"player_rate","double",false,"prim",false},
  {11,"is_public_player_rate","bool",false,"prim",false},
  {12,"league_class","LeagueClassTypes",false,"enum",false},
  {13,"total_sp_count","int",false,"prim",false},
  {14,"is_public_album_main_page","bool",false,"prim",false},
  {15,"m_nameplate_detail_id","long",false,"prim",true},
  {16,"main_character_master_id","long",false,"prim",false},
  {17,"display_awakening_status","bool",false,"prim",false},
  {18,"is_public_activity_log","bool",false,"prim",false},
  {19,"name_base_color_master_id","long",false,"prim",false},
  {20,"icon_frame_master_id","long",false,"prim",false},
  {21,"home_skin_master_id","long",false,"prim",false}
};
static const FieldSpec _k_UserProfileDetail[] = {
  {0,"user_id","string",false,"prim",true},
  {1,"user_name","string",false,"prim",true},
  {2,"trophy_master_id1","long",false,"prim",true},
  {3,"trophy_master_id2","long",false,"prim",true},
  {4,"trophy_master_id3","long",false,"prim",true},
  {5,"main_m_character_id","long",false,"prim",false},
  {6,"main_character_level","int",false,"prim",false},
  {7,"character_display_awakening_status","bool",false,"prim",false},
  {8,"icon_frame_master_id","long",false,"prim",false},
  {9,"name_color_master_id","long",false,"prim",false},
  {10,"name_base_color_master_id","long",false,"prim",false},
  {11,"nameplate_master_id","long",false,"prim",true},
  {12,"nameplate_detail_master_id","long",false,"prim",true}
};
static const FieldSpec _k_UserResult[] = {
  {0,"user","User",false,"model",true}
};
static const FieldSpec _k_ViewShopResult[] = {
  {0,"converted_things","ConvertedThingResult",true,"model",true}
};
static const FieldSpec _k_ViewedShop[] = {
  {0,"id_","long",false,"prim",false},
  {1,"exchange_shop_master_id","long",false,"prim",true},
  {2,"last_viewed_at","DateTime",false,"prim",false},
  {3,"viewed_shop_category","ViewedShopCategoryTypes",false,"enum",false}
};
static const FieldSpec _k_AccessoryEffectFilterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"effect_type","EffectTypes",false,"enum",false},
  {2,"name","string",false,"prim",true},
  {3,"start_date","DateTime",false,"prim",false},
  {4,"order","int",false,"prim",false}
};
static const FieldSpec _k_ActivityLogMessageTemplateMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"log_type","ActivityLogTypes",false,"enum",false},
  {2,"message_template","string",false,"prim",true}
};
static const FieldSpec _k_AdditionalRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"description","string",false,"prim",true},
  {2,"live_setting_master_id","long",false,"prim",false},
  {3,"rewards","AdditionalRewardThingMaster",true,"model",true},
  {4,"etcetera","bool",false,"prim",false},
  {5,"start_date","DateTime",false,"prim",true},
  {6,"end_date","DateTime",false,"prim",true},
  {7,"order","int",false,"prim",false}
};
static const FieldSpec _k_AdditionalRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_AlbumEffectMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"description","string",false,"prim",true},
  {3,"effect_master_id","long",false,"prim",false}
};
static const FieldSpec _k_AlbumThemeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"description","string",false,"prim",true},
  {4,"is_hide","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false},
  {6,"start_date","DateTime",false,"prim",false},
  {7,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_AnotherNotationMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"vocal_version","int",false,"prim",false},
  {3,"notation_path","string",false,"prim",true},
  {4,"difficulty","MusicDifficulties",false,"enum",false},
  {5,"level","int",false,"prim",false},
  {6,"another_notation_type","AnotherNotationTypes",false,"enum",false},
  {7,"start_date","DateTime",false,"prim",false},
  {8,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_BannerMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"jump_type","JumpTypes",false,"enum",false},
  {4,"jump_page_id","string",false,"prim",true},
  {5,"web_link_type","WebLinkTypes",false,"enum",true},
  {6,"link_url","string",false,"prim",true},
  {7,"delete_condition_type","BannerDeleteConditionTypes",false,"enum",false},
  {8,"delete_condition_value","string",false,"prim",true},
  {9,"image_path","string",false,"prim",true},
  {10,"start_date","DateTime",false,"prim",false},
  {11,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_BodyMotionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"motion_name","string",false,"prim",true}
};
static const FieldSpec _k_BonusLiveMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"first_clear_event_point_quantity","int",false,"prim",false},
  {3,"clear_event_point_quantity","int",false,"prim",false},
  {4,"unlock_condition_type","BonusLiveUnlockConditionTypes",false,"enum",false},
  {5,"unlock_condition_value","long",false,"prim",true},
  {6,"stages","BonusLiveStageMaster",true,"model",true}
};
static const FieldSpec _k_BonusLiveStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bonus_live_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"music_master_id","long",false,"prim",false},
  {4,"vocal_version","int",false,"prim",false},
  {5,"sense_notation_master_id","long",false,"prim",false},
  {6,"recommended_company","Companies",false,"enum",true},
  {7,"recommended_level","int",false,"prim",false},
  {8,"clear_score","long",false,"prim",false},
  {9,"unlock_condition_value","long",false,"prim",true},
  {10,"start_date","DateTime",false,"prim",false},
  {11,"rewards","BonusLiveStageRewardThingMaster",true,"model",true},
  {12,"clear_star_act_count","int",false,"prim",true}
};
static const FieldSpec _k_BonusLiveStageRewardThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"is_first_clear","bool",false,"prim",false}
};
static const FieldSpec _k_BuffItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"effect_type","CampaignEffectTypes",false,"enum",false},
  {3,"effect_value","int",false,"prim",false},
  {4,"valid_day","int",false,"prim",false},
  {5,"dialog_title","string",false,"prim",true},
  {6,"dialog_description","string",false,"prim",true}
};
static const FieldSpec _k_CategoryGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"name","string",false,"prim",true},
  {3,"color_code","string",false,"prim",true}
};
static const FieldSpec _k_CategoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"category_group_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"name","string",false,"prim",true}
};
static const FieldSpec _k_ChangeBodyMotionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"before_motion_name","string",false,"prim",true},
  {2,"after_motion_name","string",false,"prim",true},
  {3,"second","float",false,"prim",false}
};
static const FieldSpec _k_CharacterAwakeningItemGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"items","CharacterAwakeningItemMaster",true,"model",true}
};
static const FieldSpec _k_CharacterBaseBloomGenericItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"character_base_master_id","long",false,"prim",false},
  {3,"talent_bloom_item_type","TalentBloomItemTypes",false,"enum",false}
};
static const FieldSpec _k_CharacterBloomDetailMaster[] = {
  {0,"character_master_id","long",false,"prim",false},
  {1,"text","string",false,"prim",true},
  {2,"voice_path","string",false,"prim",true}
};
static const FieldSpec _k_CharacterEpisodeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"episode_master_id","long",false,"prim",false},
  {3,"episode_order","CharacterEpisodeOrder",false,"enum",false},
  {4,"required_character_level","int",false,"prim",false}
};
static const FieldSpec _k_CharacterEpisodeRelationMaster[] = {
  {0,"character_masterid","long",false,"prim",false},
  {1,"previous_story_master_ids","long",true,"prim",true},
  {2,"next_story_master_ids","long",true,"prim",true},
  {3,"side_story_character_master_ids","long",true,"prim",true},
  {4,"related_story_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_CharacterEpisodeReleaseItemGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"items","CharacterEpisodeReleaseItemMaster",true,"model",true}
};
static const FieldSpec _k_CharacterEpisodeReleaseItemMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"required_quantity","int",false,"prim",false},
  {2,"order","int",false,"prim",false}
};
static const FieldSpec _k_CharacterKeyMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"color","string",false,"prim",true},
  {3,"give_star_point","int",false,"prim",false},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false},
  {6,"required_category_count","int",false,"prim",false}
};
static const FieldSpec _k_CharacterLessonScoreRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"required_score","long",false,"prim",false},
  {3,"lesson_score_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CharacterMissionCategoryLevelMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"category_type","CharacterMissionCategoryTypes",false,"enum",false},
  {2,"level","int",false,"prim",false},
  {3,"give_star_point","int",false,"prim",false},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_CharacterMissionItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"character_base_master_id","long",false,"prim",false},
  {3,"character_mission_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CharacterPointEventCharacterRankingRewardItemMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_CharacterPointEventCharacterRankingRewardItemPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","CharacterPointEventCharacterRankingRewardItemMaster",true,"model",true}
};
static const FieldSpec _k_CharacterPointEventCharacterRankingRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_point_event_master_id","long",false,"prim",false},
  {2,"character_base_master_id","long",false,"prim",false},
  {3,"target_min_rank","int",false,"prim",false},
  {4,"target_max_rank","int",false,"prim",false},
  {5,"character_point_event_character_ranking_reward_item_package_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CharacterPointEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"title","string",false,"prim",true},
  {3,"exchange_shop_master_id","long",false,"prim",false},
  {4,"event_point_item_master_id","long",false,"prim",false},
  {5,"character_base_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_CharacterProfileRestrictionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"profile_restriction_item","CharacterProfileRestrictionItem",false,"enum",false},
  {3,"unlock_condition_master_id","long",false,"prim",false},
  {4,"profile_unlock_value","long",false,"prim",false}
};
static const FieldSpec _k_CircleEventCirclePointRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"circle_event_master_id","long",false,"prim",false},
  {2,"point","long",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_id","long",false,"prim",false},
  {5,"thing_quantity","int",false,"prim",false},
  {6,"order","int",false,"prim",false}
};
static const FieldSpec _k_CircleEventCircleRankingRewardItem[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_CircleEventCircleRankingRewardItemPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","CircleEventCircleRankingRewardItem",true,"model",true}
};
static const FieldSpec _k_CircleEventCircleRankingRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"circle_event_master_id","long",false,"prim",false},
  {2,"circle_event_circle_ranking_reward_item_package_master_id","long",false,"prim",false},
  {3,"target_min_rank","int",false,"prim",false},
  {4,"target_max_rank","int",false,"prim",false}
};
static const FieldSpec _k_CircleEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"circle_event_mission_refresh_setting_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_CircleEventMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"circle_event_master_id","long",false,"prim",false},
  {2,"mission_category","CircleEventMissionTypes",false,"enum",false},
  {3,"title","string",false,"prim",true},
  {4,"jump_type","JumpTypes",false,"enum",false},
  {5,"jump_target_id","long",false,"prim",true},
  {6,"goal_count","long",false,"prim",false},
  {7,"give_point_at_goal","int",false,"prim",false},
  {8,"circle_goal_count","long",false,"prim",false},
  {9,"circle_give_point_at_goal","int",false,"prim",false},
  {10,"is_default","bool",false,"prim",false}
};
static const FieldSpec _k_CircleEventMissionRefreshSetting[] = {
  {0,"min_refresh_count","int",false,"prim",false},
  {1,"max_refresh_count","int",false,"prim",false},
  {2,"required_coin","int",false,"prim",false}
};
static const FieldSpec _k_CircleEventMissionRefreshSettingGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"settings","CircleEventMissionRefreshSetting",true,"model",true}
};
static const FieldSpec _k_CircleSupportCompanyLevelDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"company","Companies",false,"enum",false},
  {2,"level","int",false,"prim",false},
  {3,"required_support_point","int",false,"prim",false},
  {4,"effect_master_id","long",false,"prim",false},
  {5,"description","string",false,"prim",true},
  {6,"release_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_CircleSupportCompanyLevelLimitDetailMaster[] = {
  {0,"order","int",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"required_quantity","long",false,"prim",false}
};
static const FieldSpec _k_CircleSupportCompanyLevelLimitMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"next_level_limit","int",false,"prim",false},
  {2,"required_coin","long",false,"prim",false},
  {3,"release_date","DateTime",false,"prim",false},
  {4,"details","CircleSupportCompanyLevelLimitDetailMaster",true,"model",true}
};
static const FieldSpec _k_CircleTheaterLevelMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"level","int",false,"prim",false},
  {2,"stamina_base_value","int",false,"prim",false},
  {3,"description","string",false,"prim",true}
};
static const FieldSpec _k_ComebackCampaignMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"mission_active_days","int",false,"prim",false},
  {2,"jewel_shop_item_active_days","int",false,"prim",false},
  {3,"login_bonus_active_days","int",false,"prim",false},
  {4,"buff_campaign_active_days","int",false,"prim",false}
};
static const FieldSpec _k_ComicEpisodeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"title","string",false,"prim",true},
  {3,"body","string",false,"prim",true},
  {4,"start_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_ComicMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"group_name","string",false,"prim",true},
  {2,"episodes","ComicEpisodeMaster",true,"model",true}
};
static const FieldSpec _k_ConcertMaster[] = {
  {0,"id_","long",false,"prim",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false},
  {4,"name","string",false,"prim",true},
  {5,"event_master_id","long",false,"prim",false}
};
static const FieldSpec _k_ConcertRewardMaster[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_ConcertStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"concert_master_id","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false},
  {3,"vocal_version","int",false,"prim",false},
  {4,"sense_notation_master_id","long",false,"prim",false},
  {5,"recommended_company","Companies",false,"enum",true},
  {6,"recommended_level","int",false,"prim",false},
  {7,"clear_score","long",false,"prim",false},
  {8,"rewards","ConcertRewardMaster",true,"model",true}
};
static const FieldSpec _k_ConcoursDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_ConcoursMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"bgm_path","string",false,"prim",true},
  {3,"details","ConcoursDetailMaster",true,"model",true},
  {4,"points","ConcoursPointMaster",true,"model",true}
};
static const FieldSpec _k_ConcoursPointMaster[] = {
  {0,"target_highest_rank","int",false,"prim",false},
  {1,"target_lowest_rank","int",false,"prim",false},
  {2,"point","int",false,"prim",false}
};
static const FieldSpec _k_ConcoursPointRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"concours_master_id","long",false,"prim",false},
  {2,"required_point","int",false,"prim",false},
  {3,"things","ConcoursPointRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_ConcoursPointRewardThingMaster[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_CostumeGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"order","int",false,"prim",false},
  {4,"costume_master_ids","long",true,"prim",true},
  {5,"costume_wearable_character_group_master_id","long",false,"prim",true},
  {6,"performance_group_id","int",false,"prim",true},
  {7,"is_hair_change","bool",false,"prim",false},
  {8,"display_start_at","DateTime",false,"prim",false},
  {9,"display_end_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_CourseRankingRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_course_master_id","long",false,"prim",false},
  {2,"target_highest_rank","int",false,"prim",false},
  {3,"target_lowest_rank","int",false,"prim",false},
  {4,"rewards","CourseRankingRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_CourseRankingRewardThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_DecorationMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"is_default","bool",false,"prim",false},
  {3,"name","string",false,"prim",true},
  {4,"category","DecorationCategories",false,"enum",false},
  {5,"widht_px","int",false,"prim",false},
  {6,"height_px","int",false,"prim",false},
  {7,"min_width_px","int",false,"prim",false},
  {8,"max_width_px","int",false,"prim",false}
};
static const FieldSpec _k_DugongRunCourseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",true},
  {2,"difficulty","DugongRunDifficultyTypes",false,"enum",false},
  {3,"music_master_id","long",false,"prim",false},
  {4,"notation_path","string",false,"prim",true},
  {5,"scroll_speed","int",false,"prim",false},
  {6,"dugong_run_course_group_id","long",false,"prim",false}
};
static const FieldSpec _k_DugongRunRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"required_clear_course_count","int",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_id","long",false,"prim",false},
  {4,"thing_quantity","int",false,"prim",false},
  {5,"dugong_run_course_group_id","long",false,"prim",false}
};
static const FieldSpec _k_EffectTriggerCharacterBaseGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_EventBonusMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"bonus_target_type","StoryEventBonusTypes",false,"enum",false},
  {2,"bonus_target_id","long",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaBoxMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_box_gacha_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"box_things","EventBoxGachaBoxThingMaster",true,"model",true}
};
static const FieldSpec _k_EventBoxGachaBoxThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"hit_limit","int",false,"prim",false},
  {5,"is_resettable","bool",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"prize_count","int",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"event_master_id","long",false,"prim",false},
  {3,"required_item_master_id","long",false,"prim",false},
  {4,"required_quantity","int",false,"prim",false},
  {5,"details","EventBoxGachaDetailMaster",true,"model",true},
  {6,"event_box_gacha_text_template_master_id","long",false,"prim",false}
};
static const FieldSpec _k_EventBoxGachaTextTemplateMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"description","string",false,"prim",true}
};
static const FieldSpec _k_EventCampClassMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"target_max_rank","int",false,"prim",false},
  {3,"target_min_rank","int",false,"prim",false},
  {4,"reward_receive_ratio","int",false,"prim",false}
};
static const FieldSpec _k_EventCampMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"win_camp_ratio","int",false,"prim",false},
  {3,"lose_camp_ratio","int",false,"prim",false},
  {4,"none_camp_ratio","int",false,"prim",false},
  {5,"camp1_team_name","string",false,"prim",true},
  {6,"camp2_team_name","string",false,"prim",true},
  {7,"camp_select_message","string",false,"prim",true},
  {8,"bgm_path","string",false,"prim",true},
  {9,"character_master_id_icon_left","long",false,"prim",false},
  {10,"character_master_id_icon_right","long",false,"prim",false}
};
static const FieldSpec _k_EventCampSupportPointRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"target_min_rank","int",false,"prim",false},
  {3,"target_max_rank","int",false,"prim",false},
  {4,"camp1_reward_trophy_master_id","long",false,"prim",false},
  {5,"camp2_reward_trophy_master_id","long",false,"prim",false}
};
static const FieldSpec _k_EventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_type","EventTypes",false,"enum",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false},
  {4,"force_end_date","DateTime",false,"prim",false},
  {5,"name","string",false,"prim",true},
  {6,"is_aside_live_button","bool",false,"prim",false},
  {7,"event_point_item_master_id","long",false,"prim",true},
  {8,"bonus_attribute","Attributes",false,"enum",true},
  {9,"bonus_category_master_id10","long",false,"prim",true},
  {10,"bonus_category_master_id20","long",false,"prim",true},
  {11,"bonuses","EventBonusMaster",true,"model",true},
  {12,"point_rewards","EventPointRewardMaster",true,"model",true},
  {13,"ranking_rewards","EventRankingRewardMaster",true,"model",true},
  {14,"exchange_shop_master_id","long",false,"prim",true},
  {15,"aside_live_button_type","AsideLiveButtonTypes",false,"enum",false},
  {16,"secondary_bonus_attribute","Attributes",false,"enum",true}
};
static const FieldSpec _k_EventPointRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"required_point","int",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_id","long",false,"prim",false},
  {4,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_EventRankingRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"target_highest_rank","int",false,"prim",false},
  {2,"target_lowest_rank","int",false,"prim",false},
  {3,"rewards","EventRankingRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_EventRankingRewardThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"is_camp_reward","bool",false,"prim",false}
};
static const FieldSpec _k_FacialExpressionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"eye_brow","string",false,"prim",true},
  {2,"eye","string",false,"prim",true},
  {3,"eye_blink","string",false,"prim",true},
  {4,"cheek","string",false,"prim",true},
  {5,"mouth","string",false,"prim",true}
};
static const FieldSpec _k_FilmItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"target_character_base_master_id","long",false,"prim",true}
};
static const FieldSpec _k_FriendInvitationMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"description","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",false},
  {4,"end_date","DateTime",false,"prim",false},
  {5,"jump_type","JumpTypes",false,"enum",false},
  {6,"jump_target_id","long",false,"prim",true},
  {7,"stages","FriendInvitationMissionStageMaster",true,"model",true}
};
static const FieldSpec _k_FriendInvitationMissionRewardMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_FriendInvitationMissionStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"goal_count","int",false,"prim",false},
  {2,"mission_stage_order","int",false,"prim",false},
  {3,"rewards","FriendInvitationMissionRewardMaster",true,"model",true}
};
static const FieldSpec _k_GachaBonusThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_GachaDetailBonusThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_GachaDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"required_ticket_m_item_id","long",false,"prim",true},
  {2,"required_ticket_quantity","int",false,"prim",true},
  {3,"free_jewel_amount","int",false,"prim",true},
  {4,"paid_jewel_amount","int",false,"prim",true},
  {5,"is_free","bool",false,"prim",false},
  {6,"daily_roll_limit","int",false,"prim",true},
  {7,"overall_roll_limit","int",false,"prim",true},
  {8,"prize_count","int",false,"prim",false},
  {9,"fixed_prize_count","int",false,"prim",false},
  {10,"button_type","GachaButtonTypes",false,"enum",false},
  {11,"detail_bonus_things","GachaDetailBonusThing",true,"model",true}
};
static const FieldSpec _k_GachaMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"card_type","GachaCardTypes",false,"enum",false},
  {3,"gacha_type","GachaTypes",false,"enum",false},
  {4,"gacha_text_template_master_id","long",false,"prim",false},
  {5,"start_date","DateTime",false,"prim",false},
  {6,"end_date","DateTime",false,"prim",false},
  {7,"bonus_things","GachaBonusThing",true,"model",true},
  {8,"gacha_details","GachaDetailMaster",true,"model",true},
  {9,"things","GachaThing",true,"model",true},
  {10,"has_movie","bool",false,"prim",false},
  {11,"story_event_master_id","long",false,"prim",true},
  {12,"attention_gacha_text_template_master_id","long",false,"prim",false},
  {13,"is_force_display","bool",false,"prim",false},
  {14,"is_hide_end_date","bool",false,"prim",false},
  {15,"unlock_type","GachaUnlockTypes",false,"enum",false},
  {16,"unlock_value","long",false,"prim",true},
  {17,"exchange_shop_banner_path","string",false,"prim",true},
  {18,"order","long",false,"prim",false},
  {19,"display_footer_condition","GachaDisplayFooterConditions",false,"enum",true},
  {20,"group_type","GachaGroupTypes",false,"enum",false},
  {21,"roll_bonuses","GachaRollBonus",true,"model",true},
  {22,"re_roll_limit","int",false,"prim",true},
  {23,"replace_asset_name","string",false,"prim",true}
};
static const FieldSpec _k_GachaRollBonus[] = {
  {0,"id_","long",false,"prim",false},
  {1,"roll_count","int",false,"prim",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_GachaTextTemplateMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"description","string",false,"prim",true}
};
static const FieldSpec _k_GachaThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"pickup_order","int",false,"prim",true},
  {5,"is_selectable","bool",false,"prim",false}
};
static const FieldSpec _k_GameHintMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"page_category","PageCategories",false,"enum",false},
  {2,"page_count","int",false,"prim",false}
};
static const FieldSpec _k_GhostLiveMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"vocal_version","int",false,"prim",false},
  {3,"sense_notation_master_id","long",false,"prim",false},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_GradualMissionGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"start_date","DateTime",false,"prim",false},
  {2,"end_date","DateTime",false,"prim",false},
  {3,"missions","GradualMissionMaster",true,"model",true}
};
static const FieldSpec _k_GradualMissionMaster[] = {
  {0,"days","long",false,"prim",false},
  {2,"mission_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_HeadDirectionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"direction_name","string",false,"prim",true}
};
static const FieldSpec _k_HeadMotionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"motion_name","string",false,"prim",true}
};
static const FieldSpec _k_HomeBGMDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"home_bgm_master_id","long",false,"prim",false},
  {2,"bgm_name","string",false,"prim",true},
  {3,"bgm_path","string",false,"prim",true},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false},
  {6,"music_master_id","long",false,"prim",false}
};
static const FieldSpec _k_HomeBGMMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"priority","HomeBGMPriorityTypes",false,"enum",false},
  {2,"selection_type","HomeBGMSelectionTypes",false,"enum",false},
  {3,"default_bgm_name","string",false,"prim",true},
  {4,"default_bgm_path","string",false,"prim",true},
  {5,"start_date","DateTime",false,"prim",false},
  {6,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_HomeBackgroundMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"start_date","DateTime",false,"prim",true},
  {3,"end_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_HomeCharacterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_HomeCharacterVoicePeriodMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"start_month","int",false,"prim",false},
  {2,"start_day","int",false,"prim",false},
  {3,"start_time","TimeSpan",false,"prim",false},
  {4,"end_month","int",false,"prim",false},
  {5,"end_day","int",false,"prim",false},
  {6,"end_time","TimeSpan",false,"prim",false}
};
static const FieldSpec _k_HomePosterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"poster_master_id","long",false,"prim",false},
  {2,"gacha_master_id","long",false,"prim",true},
  {3,"display_start_date","DateTime",false,"prim",false},
  {4,"display_end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_HomeSkinMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"unlock_condition_text","string",false,"prim",true},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false},
  {6,"display_start_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_IconFrameMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"unlock_condition_text","string",false,"prim",true},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false}
};
static const FieldSpec _k_JewelShopCategoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"layout_type","int",false,"prim",false},
  {3,"is_display_locking","int",false,"prim",true},
  {4,"banner_path","string",false,"prim",true},
  {5,"order","int",false,"prim",false}
};
static const FieldSpec _k_JewelShopItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"app_store_product_id","string",false,"prim",true},
  {3,"google_play_product_id","string",false,"prim",true},
  {4,"name","string",false,"prim",true},
  {5,"pack_text","string",false,"prim",true},
  {6,"m_jewel_shop_category_id","long",false,"prim",false},
  {7,"dialog_type","int",false,"prim",false},
  {8,"purchase_type","ShopPurchaseTypes",false,"enum",false},
  {9,"purchase_value","int",false,"prim",true},
  {10,"subscription_flag","bool",false,"prim",false},
  {11,"give_paid_jewel","int",false,"prim",true},
  {12,"replace_types","ShopReplaceTypes",false,"enum",true},
  {13,"replace_value","int",false,"prim",true},
  {14,"purchase_limit","int",false,"prim",true},
  {15,"expired_day","int",false,"prim",true},
  {16,"can_purchase_day_of_week","string",false,"prim",true},
  {17,"is_buy_not_display","bool",false,"prim",false},
  {18,"sale_type","SaleTypes",false,"enum",true},
  {19,"sale_value","int",false,"prim",true},
  {20,"is_display_locking","bool",false,"prim",false},
  {21,"unlock_type","JewelShopUnlockTypes",false,"enum",true},
  {22,"unlock_value","long",false,"prim",true},
  {23,"start_date","DateTime",false,"prim",true},
  {24,"end_date","DateTime",false,"prim",true},
  {25,"lineup","JewelShopThing",true,"model",true},
  {26,"shop_item_type","ShopItemTypes",false,"enum",false},
  {27,"shop_item_value","long",false,"prim",true},
  {28,"comeback_campaign_master_id","long",false,"prim",true},
  {29,"item_icon_body_image_path","string",false,"prim",true},
  {30,"item_icon_detail_image_path","string",false,"prim",true},
  {31,"item_icon_badge_image_path","string",false,"prim",true}
};
static const FieldSpec _k_JewelShopThing[] = {
  {0,"id_","long",false,"prim",false},
  {1,"m_jewel_shop_item_id","long",false,"prim",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LeaderSenseDetailConditionMaster[] = {
  {0,"category_master_id1","long",false,"prim",true},
  {1,"category_master_id2","long",false,"prim",true},
  {2,"category_master_id3","long",false,"prim",true},
  {3,"category_master_id4","long",false,"prim",true},
  {4,"category_master_id5","long",false,"prim",true}
};
static const FieldSpec _k_LeaderSenseDetailMaster[] = {
  {0,"effect_master_id","long",false,"prim",false},
  {1,"conditions","LeaderSenseDetailConditionMaster",true,"model",true}
};
static const FieldSpec _k_LeaderSenseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"description","string",false,"prim",true},
  {2,"details","LeaderSenseDetailMaster",true,"model",true}
};
static const FieldSpec _k_LeagueAllClassGlobalRankingRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LeagueClassGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"classes","LeagueClassMaster",true,"model",true}
};
static const FieldSpec _k_LeagueClassMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"up_user_amount","int",false,"prim",true},
  {2,"keep_user_amount","int",false,"prim",true},
  {3,"is_force_down_not_playing_league","bool",false,"prim",false},
  {4,"up_reward_package_master_id","long",false,"prim",true},
  {5,"keep_reward_package_master_id","long",false,"prim",true},
  {6,"down_reward_package_master_id","long",false,"prim",true},
  {7,"first_achieve_reward_package_master_id","long",false,"prim",true},
  {8,"nameplate_master_id","long",false,"prim",true},
  {9,"class_type","LeagueClassTypes",false,"enum",false},
  {10,"league_class_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_LeagueGroupRankingRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","LeagueGroupRankingRewardThingMaster",true,"model",true},
  {2,"class_type","LeagueClassTypes",false,"enum",false},
  {3,"min_rank","int",false,"prim",false},
  {4,"max_rank","int",false,"prim",false},
  {5,"reward_title","string",false,"prim",true}
};
static const FieldSpec _k_LeagueGroupRankingRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LeagueMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"max_difficulty","MusicDifficulties",false,"enum",false},
  {3,"display_start_at","DateTime",false,"prim",false},
  {4,"counting_start_at","DateTime",false,"prim",false},
  {5,"display_end_at","DateTime",false,"prim",false},
  {6,"league_class_group_master_id","long",false,"prim",false},
  {7,"sense_notation_master_id","long",false,"prim",false},
  {8,"vocal_version","int",false,"prim",false},
  {9,"league_season_master_id","long",false,"prim",false}
};
static const FieldSpec _k_LeaguePlayRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","LeaguePlayRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_LeaguePlayRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LeagueRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","LeagueRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_LeagueRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LessonScoreRewardGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","LessonScoreRewardMaster",true,"model",true}
};
static const FieldSpec _k_LessonScoreRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_LightLoadSplitEffectMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"base_effect","int",false,"prim",false},
  {2,"light_load_effect","int",false,"prim",false}
};
static const FieldSpec _k_LipSyncMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"motion_name","string",false,"prim",true}
};
static const FieldSpec _k_LiveDropFrameGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"drop_frames","LiveDropFrameMaster",true,"model",true}
};
static const FieldSpec _k_LiveDropFrameMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"live_drop_frame_group_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"change_type","ProbabilityChangeTypes",false,"enum",false},
  {4,"frame_lot_condition","FrameLotConditionTypes",false,"enum",false},
  {5,"unaffected_increase_effect","bool",false,"prim",false},
  {6,"rewards","LiveDropThingMaster",true,"model",true},
  {7,"frame_lot_condition_value","int",false,"prim",true},
  {8,"start_date","DateTime",false,"prim",true},
  {9,"end_date","DateTime",false,"prim",true}
};
static const FieldSpec _k_LiveDropThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false},
  {3,"is_special_fall_count_up_thing","bool",false,"prim",false},
  {4,"is_special_fall_thing","bool",false,"prim",false}
};
static const FieldSpec _k_LoginBonusSpineCostumeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"spine_id","long",false,"prim",false},
  {2,"character_base_master_id","long",false,"prim",false},
  {3,"target_value","long",false,"prim",false},
  {4,"release_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_LoopMotionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"target_character_base_id","string",false,"prim",true},
  {2,"loop_speed","float",false,"prim",false},
  {3,"height","int",false,"prim",false},
  {4,"size","SpineBodySizeTypes",false,"enum",false}
};
static const FieldSpec _k_MarketFrameThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {6,"required_thing_type","ThingTypes",false,"enum",false},
  {7,"required_thing_id","long",false,"prim",false},
  {8,"required_thing_quantity","int",false,"prim",false},
  {9,"display_type","MarketDisplayTypes",false,"enum",false}
};
static const FieldSpec _k_MissionPassLoopRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"phase","int",false,"prim",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false},
  {4,"rewards","MissionPassLoopRewardThingMaster",true,"model",true}
};
static const FieldSpec _k_MissionPassLoopRewardThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"mission_pass_loop_reward_master_id","long",false,"prim",false},
  {2,"thing_id","long",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_quantity","int",false,"prim",false},
  {5,"is_sp","bool",false,"prim",false}
};
static const FieldSpec _k_MultiLiveScheduleMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"start_date","DateTime",false,"prim",false},
  {2,"end_date","DateTime",false,"prim",false},
  {3,"live_setting_master_id1","long",false,"prim",false},
  {4,"live_setting_master_id2","long",false,"prim",false},
  {5,"additional_end_date","DateTime",false,"prim",true},
  {6,"end_date_display_type1","LiveScheduleEndDateDisplayType",false,"enum",false},
  {7,"end_date_display_type2","LiveScheduleEndDateDisplayType",false,"enum",false}
};
static const FieldSpec _k_MusicCourseDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"set_list_number","int",false,"prim",false},
  {2,"live_master_id","long",false,"prim",true},
  {3,"vocal_version","int",false,"prim",false},
  {4,"another_notation_master_id","long",false,"prim",false},
  {5,"random_difficulty","MusicDifficulties",false,"enum",true},
  {6,"random_level","int",false,"prim",true}
};
static const FieldSpec _k_MusicCourseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"music_course_type","MusicCourseType",false,"enum",false},
  {3,"order","long",false,"prim",false},
  {4,"initial_life","int",false,"prim",false},
  {5,"required_item_master_id","long",false,"prim",true},
  {6,"required_amount","int",false,"prim",false},
  {7,"start_date","DateTime",false,"prim",false},
  {8,"end_date","DateTime",false,"prim",true},
  {9,"unlock_condition","MusicCourseUnlockConditionTypes",false,"enum",false},
  {10,"unlock_condition_value","long",false,"prim",true},
  {11,"tournament_qualifying_master_id","long",false,"prim",true},
  {12,"details","MusicCourseDetailMaster",true,"model",true}
};
static const FieldSpec _k_MusicCourseRewardGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_course_master_id","long",false,"prim",false},
  {2,"required_certification_grade","MusicCourseCertificationGrade",false,"enum",false},
  {3,"rewards","MusicCourseRewardThing",true,"model",true}
};
static const FieldSpec _k_MusicCourseRewardThing[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_MusicGroupDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_group_master_id","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MusicGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"main_music_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MusicVideoDefaultCostumeGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"default_costumes","MusicVideoDefaultCostumeMaster",true,"model",true}
};
static const FieldSpec _k_MusicVideoDefaultCostumeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"costume_master_id","long",false,"prim",false}
};
static const FieldSpec _k_MusicVideoMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"member_amount","int",false,"prim",false},
  {3,"display_start_at","DateTime",false,"prim",false},
  {4,"display_end_at","DateTime",false,"prim",true},
  {5,"characters","MusicVideoOriginalCharacterMaster",true,"model",true},
  {6,"delay_seconds","float",false,"prim",false},
  {7,"movie_delay_seconds","float",false,"prim",false},
  {8,"music_video_default_costume_group_master_id","long",false,"prim",true},
  {9,"is_fixed_member","bool",false,"prim",false},
  {10,"order","int",false,"prim",false}
};
static const FieldSpec _k_MusicVideoOriginalCharacterMaster[] = {
  {0,"order","int",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"costume_master_id","long",false,"prim",false}
};
static const FieldSpec _k_NameBaseColorMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"unlock_condition_text","string",false,"prim",true},
  {4,"hidden","bool",false,"prim",false},
  {5,"is_default","bool",false,"prim",false}
};
static const FieldSpec _k_NgWordMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"key_word","string",false,"prim",true}
};
static const FieldSpec _k_PermanentMarketThingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_type","ThingTypes",false,"enum",false},
  {3,"thing_quantity","int",false,"prim",false},
  {4,"exchange_limit","int",false,"prim",true},
  {5,"order","int",false,"prim",false},
  {6,"unlock_type","ShopUnlockTypes",false,"enum",true},
  {7,"unlock_value","long",false,"prim",true},
  {8,"start_date","DateTime",false,"prim",true},
  {9,"end_date","DateTime",false,"prim",true},
  {11,"required_thing_type","ThingTypes",false,"enum",false},
  {12,"required_thing_id","long",false,"prim",false},
  {13,"required_thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PhotoEffectChangeItemMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"item_master_id","long",false,"prim",false},
  {2,"photo_effect_type_group_master_id","long",false,"prim",false},
  {3,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PhotoEffectMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"effect_master_id","long",false,"prim",false},
  {4,"variety","int",false,"prim",true},
  {5,"photo_effect_type_group_master_id","long",false,"prim",true},
  {6,"photo_effect_group_master_id","long",false,"prim",false}
};
static const FieldSpec _k_PhotoEffectTypeGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"photo_effect_max_variety","int",false,"prim",false},
  {2,"description","string",false,"prim",true},
  {3,"can_use_item","bool",false,"prim",false}
};
static const FieldSpec _k_PhotoEffectVarietyChangeDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"before_photo_effect_max_variety","int",false,"prim",false},
  {2,"after_photo_effect_max_variety","int",false,"prim",false},
  {3,"before_variety","int",false,"prim",false},
  {4,"after_variety","int",false,"prim",false}
};
static const FieldSpec _k_PhotoEffectVarietyUpDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"photo_effect_type_group_master_id","long",false,"prim",false},
  {2,"current_variety","int",false,"prim",false},
  {3,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PhotoLevelUpItemGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rarity","PhotoRarities",false,"enum",false},
  {2,"base_level","int",false,"prim",false},
  {3,"required_coin","int",false,"prim",false},
  {4,"items","PhotoLevelUpItemMaster",true,"model",true}
};
static const FieldSpec _k_PhotoLevelUpItemMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"required_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PhotoSpotMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"release_player_rank","int",false,"prim",false},
  {3,"display_start_at","DateTime",false,"prim",false},
  {4,"display_end_at","DateTime",false,"prim",false}
};
static const FieldSpec _k_PickupCharacterMissionDetailGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"details","PickupCharacterMissionDetailMaster",true,"model",true}
};
static const FieldSpec _k_PickupCharacterMissionDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"check_condition","PickupCharacterMissionCheckCondition",false,"enum",false},
  {3,"description","string",false,"prim",true},
  {4,"goal_value","int",false,"prim",false},
  {5,"rewards","PickupCharacterMissionDetailRewardMaster",true,"model",true}
};
static const FieldSpec _k_PickupCharacterMissionDetailRewardMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_PickupCharacterMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"pickup_character_mission_detail_group_master_id","long",false,"prim",false},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_PickupSelectionGachaMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"gacha_master_id","long",false,"prim",false},
  {2,"min_select_count","int",false,"prim",false},
  {3,"max_select_count","int",false,"prim",false},
  {4,"fixed_prize_pickup_only","bool",false,"prim",false}
};
static const FieldSpec _k_PlayerRankCapDetailMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"quantity","int",false,"prim",false}
};
static const FieldSpec _k_PlayerRankCapMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"coin","int",false,"prim",false},
  {2,"details","PlayerRankCapDetailMaster",true,"model",true}
};
static const FieldSpec _k_PlayerRankMaster[] = {
  {0,"rank","int",false,"prim",false},
  {1,"point_to_level_up","int",false,"prim",false},
  {2,"max_stamina","int",false,"prim",false},
  {4,"is_released_rank","bool",false,"prim",false},
  {5,"player_rank_cap_master_id","long",false,"prim",true}
};
static const FieldSpec _k_PosterAbilityMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {3,"poster_master_id","long",false,"prim",false},
  {4,"type","PosterEffectTypes",false,"enum",false},
  {6,"frame_number","int",false,"prim",false},
  {7,"release_level_at","int",false,"prim",false},
  {8,"hidden","bool",false,"prim",false},
  {9,"branches","BranchMaster",true,"model",true},
  {10,"branch_condition_type1","BranchConditionType",false,"enum",false},
  {11,"condition_value1","long",false,"prim",true},
  {12,"branch_condition_type2","BranchConditionType",false,"enum",false},
  {13,"condition_value2","long",false,"prim",true}
};
static const FieldSpec _k_ResultVoiceMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"condition_type","ResultVoiceConditionTypes",false,"enum",false},
  {4,"voice_asset_id","string",false,"prim",true},
  {5,"motion","string",false,"prim",true}
};
static const FieldSpec _k_RouletteEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"roulette_point_item_master_id","long",false,"prim",false}
};
static const FieldSpec _k_RouletteMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"roulette_event_master_id","long",false,"prim",false},
  {2,"order","int",false,"prim",false},
  {3,"roulette_type","RouletteTypes",false,"enum",false},
  {4,"roulette_ticket_item_master_id","long",false,"prim",false},
  {5,"required_ticket_quantity","int",false,"prim",false},
  {6,"roulette_image_path","string",false,"prim",true},
  {7,"prizes","RoulettePrizeMaster",true,"model",true},
  {8,"roll_rewards","RouletteRollRewardMaster",true,"model",true},
  {9,"name","string",false,"prim",true}
};
static const FieldSpec _k_RoulettePrizeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"slot","int",false,"prim",false},
  {2,"acquire_point_quantity","int",false,"prim",true},
  {3,"acquire_prize_thing","RoulettePrizeThingMaster",false,"model",true}
};
static const FieldSpec _k_RoulettePrizeThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_RouletteRollRewardMaster[] = {
  {0,"roll_count","int",false,"prim",false},
  {1,"rewards","RouletteRollRewardThing",true,"model",true}
};
static const FieldSpec _k_RouletteRollRewardThing[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_SceneCameraMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"start_position_x","int",false,"prim",false},
  {2,"start_position_y","int",false,"prim",false},
  {3,"start_zoom_ratio","int",false,"prim",false},
  {4,"end_position_x","int",false,"prim",false},
  {5,"end_position_y","int",false,"prim",false},
  {6,"end_zoom_ratio","int",false,"prim",false},
  {7,"camera_move_turnaround_time_seconds","int",false,"prim",false}
};
static const FieldSpec _k_SenseBranchMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"effects","SenseEffectMaster",true,"model",true}
};
static const FieldSpec _k_SenseNotationBuffMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"type","SenseNotationBuffTypes",false,"enum",false},
  {2,"target_value","long",false,"prim",true},
  {3,"buff_value","int",false,"prim",false},
  {4,"status_type","SenseNotationStatusTypes",false,"enum",false}
};
static const FieldSpec _k_SenseNotationDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"position","int",false,"prim",false},
  {2,"timing_second","int",false,"prim",false}
};
static const FieldSpec _k_SenseNotationMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"details","SenseNotationDetailMaster",true,"model",true},
  {2,"buffs","SenseNotationBuffMaster",true,"model",true}
};
static const FieldSpec _k_SensePerformanceCharacterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"sense_performance_master_id","long",false,"prim",false},
  {3,"express_id","string",false,"prim",true}
};
static const FieldSpec _k_SensePerformanceMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"label_type","int",false,"prim",false},
  {2,"performance_group","int",false,"prim",true},
  {3,"characters","SensePerformanceCharacterMaster",true,"model",true}
};
static const FieldSpec _k_SignMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"sign_image_path","string",false,"prim",true}
};
static const FieldSpec _k_SpecialEpisodeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_master_id","long",false,"prim",false},
  {2,"title","string",false,"prim",true},
  {3,"required_read_episode_master_id","long",false,"prim",true}
};
static const FieldSpec _k_SpecialEventLayout[] = {
  {0,"id_","long",false,"prim",false},
  {1,"ui_type","CustomLayoutUITypes",false,"enum",false},
  {2,"ui_value","string",false,"prim",true},
  {3,"anchor_x_percent","int",false,"prim",false},
  {4,"anchor_y_percent","int",false,"prim",false},
  {5,"position_x","float",false,"prim",false},
  {6,"position_y","float",false,"prim",false},
  {7,"size_x","float",false,"prim",false},
  {8,"size_y","float",false,"prim",false},
  {9,"scale_x","float",false,"prim",false},
  {10,"scale_y","float",false,"prim",false},
  {11,"layer","int",false,"prim",false},
  {12,"action_type","CustomLayoutActionTypes",false,"enum",false},
  {13,"action_value","string",false,"prim",true},
  {14,"jump_type","JumpTypes",false,"enum",false},
  {15,"jump_value","string",false,"prim",true},
  {16,"start_date","DateTime",false,"prim",false},
  {17,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_SpecialEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master","EventMaster",false,"model",true},
  {2,"event_name","string",false,"prim",true},
  {3,"category_type","SpecialEventCategoryTypes",false,"enum",false},
  {4,"layouts","SpecialEventLayout",true,"model",true},
  {5,"game_hint_page_count","int",false,"prim",false}
};
static const FieldSpec _k_SpecialStoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_master_id","long",false,"prim",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false},
  {4,"event_story_list_flags","EventStoryListFlags",false,"enum",false},
  {5,"title","string",false,"prim",true},
  {6,"order","int",false,"prim",false}
};
static const FieldSpec _k_SplashMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"splash_type","SplashTypes",false,"enum",false},
  {3,"splash_value","string",false,"prim",true},
  {4,"start_date","DateTime",false,"prim",false},
  {5,"end_date","DateTime",false,"prim",false},
  {6,"additional_splash_value0","SplashAdditionalValueTypes",false,"enum",false},
  {7,"additional_splash_value1","SplashAdditionalValueTypes",false,"enum",false},
  {8,"unlock_condition","SplashUnlockConditionTypes",false,"enum",false},
  {9,"unlock_condition_value","long",false,"prim",true}
};
static const FieldSpec _k_StaminaRecoveryItemMaster[] = {
  {0,"item_master_id","long",false,"prim",false},
  {1,"recovery_amount","int",false,"prim",false},
  {2,"expired_at","DateTime",false,"prim",true},
  {3,"is_force_show_list","bool",false,"prim",false}
};
static const FieldSpec _k_StarActBranchMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"order","int",false,"prim",false},
  {2,"effects","SenseEffectMaster",true,"model",true}
};
static const FieldSpec _k_StarActConditionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"free_light","int",false,"prim",false},
  {2,"support_light","int",false,"prim",false},
  {3,"control_light","int",false,"prim",false},
  {4,"amplification_light","int",false,"prim",false},
  {5,"special_light","int",false,"prim",false}
};
static const FieldSpec _k_StarActMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"description","string",false,"prim",true},
  {4,"star_act_condition_master_id","long",false,"prim",false},
  {5,"acquirable_score_percent","int",false,"prim",false},
  {6,"score_up_per_level","int",false,"prim",false},
  {7,"pre_effects","EffectOrderMaster",true,"model",true},
  {8,"branches","BranchMaster",true,"model",true},
  {9,"branch_condition1","BranchConditionType",false,"enum",false},
  {10,"condition_value1","long",false,"prim",true},
  {11,"branch_condition2","BranchConditionType",false,"enum",false},
  {12,"condition_value2","long",false,"prim",true}
};
static const FieldSpec _k_StepupGachaGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"stepup_gachas","StepupGachaMaster",true,"model",true}
};
static const FieldSpec _k_StepupGachaMaster[] = {
  {0,"gacha_master_id","long",false,"prim",false},
  {1,"step","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventBonusCharacterBaseMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"bonus_target_type","StoryEventBonusTypes",false,"enum",false},
  {3,"bonus_target_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircleHighScoreRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"score","long",false,"prim",false},
  {3,"thing_id","long",false,"prim",false},
  {4,"thing_type","ThingTypes",false,"enum",false},
  {5,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircleMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircleMissionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"title","string",false,"prim",true},
  {3,"count_unit","string",false,"prim",true},
  {4,"goal_count","long",false,"prim",false},
  {5,"max_phase","int",false,"prim",false},
  {6,"give_point_at_goal","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventCircleMissionRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"required_circle_point","int",false,"prim",false},
  {3,"required_user_point","int",false,"prim",false},
  {4,"thing_id","long",false,"prim",false},
  {5,"thing_type","ThingTypes",false,"enum",false},
  {6,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventEpisodeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_master_id","long",false,"prim",false},
  {3,"required_total_acquired_point","long",false,"prim",false},
  {4,"title","string",false,"prim",true},
  {5,"order","int",false,"prim",false},
  {6,"required_read_episode_master_id","long",false,"prim",true},
  {7,"episode_reward_package_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScoreBuffMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_high_score_buff_pattern_group_id","long",false,"prim",false},
  {2,"effect_master_id","long",false,"prim",false},
  {3,"effect_name","string",false,"prim",true},
  {4,"effect_description","string",false,"prim",true},
  {5,"icon_image_path","string",false,"prim",true}
};
static const FieldSpec _k_StoryEventHighScoreBuffPatternGroupMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"patterns","StoryEventHighScoreBuffPatternMaster",true,"model",true}
};
static const FieldSpec _k_StoryEventHighScoreBuffPatternMaster[] = {
  {0,"next_level","int",false,"prim",false},
  {1,"required_point","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScoreBuffSettingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_high_score_master_id","long",false,"prim",false},
  {2,"story_event_high_score_buff_master_id","long",false,"prim",false},
  {3,"story_event_high_score_buff_master","StoryEventHighScoreBuffMaster",false,"model",true}
};
static const FieldSpec _k_StoryEventHighScoreMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false},
  {3,"buff_ids","long",true,"prim",true},
  {4,"sense_notation_master_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventHighScoreRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"score","long",false,"prim",false},
  {3,"thing_id","long",false,"prim",false},
  {4,"thing_type","ThingTypes",false,"enum",false},
  {5,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"category","StoryEventCategoryTypes",false,"enum",false},
  {2,"bonus_attribute","Attributes",false,"enum",true},
  {3,"title","string",false,"prim",true},
  {4,"exchange_shop_master_id","long",false,"prim",false},
  {5,"event_point_item_master_id","long",false,"prim",false},
  {6,"start_date","DateTime",false,"prim",false},
  {7,"end_date","DateTime",false,"prim",false},
  {8,"force_end_date","DateTime",false,"prim",false},
  {9,"bonus_category_master_id10","long",false,"prim",true},
  {10,"bonus_category_master_id20","long",false,"prim",true},
  {11,"secondary_bonus_attribute","Attributes",false,"enum",true}
};
static const FieldSpec _k_StoryEventRewardItemPackage[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","StoryEventRewardThing",true,"model",true}
};
static const FieldSpec _k_StoryEventRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"target_min_rank","int",false,"prim",false},
  {3,"target_max_rank","int",false,"prim",false},
  {4,"story_event_reward_item_package_id","long",false,"prim",false}
};
static const FieldSpec _k_StoryEventRewardThing[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_StoryEventStoryBgmGroupMaster[] = {
  {0,"story_event_master_id","long",false,"prim",false},
  {1,"schedules","StoryEventStoryBgmSchedule",true,"model",true}
};
static const FieldSpec _k_StoryEventStoryBgmSchedule[] = {
  {0,"asset_name","string",false,"prim",true},
  {1,"start_date","DateTime",false,"prim",false},
  {2,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_StoryEventStoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_master_id","long",false,"prim",false},
  {2,"story_event_master_id","long",false,"prim",false},
  {3,"listed_start_date","DateTime",false,"prim",false},
  {4,"event_story_list_flags","EventStoryListFlags",false,"enum",false},
  {5,"is_key_story","bool",false,"prim",false}
};
static const FieldSpec _k_StoryEventTotalPointRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"story_event_master_id","long",false,"prim",false},
  {2,"required_total_point","int",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_id","long",false,"prim",false},
  {5,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_StoryRelationMaster[] = {
  {0,"story_master_id","long",false,"prim",false},
  {1,"previous_story_master_ids","long",true,"prim",true},
  {2,"next_story_master_ids","long",true,"prim",true},
  {3,"side_story_character_master_ids","long",true,"prim",true},
  {4,"related_story_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_TeamChallengeMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"difficulty","TeamChallengeDifficultyTypes",false,"enum",false},
  {3,"goal_type","TeamChallengeGoalTypes",false,"enum",false},
  {4,"goal_min_value","long",false,"prim",false},
  {5,"goal_max_value","long",false,"prim",false},
  {6,"start_date","DateTime",false,"prim",false},
  {7,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_TheaterChapterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"company_master_id","long",false,"prim",false},
  {3,"display_start_at","DateTime",false,"prim",false},
  {4,"display_end_at","DateTime",false,"prim",false},
  {5,"stories","TheaterStoryMaster",true,"model",true},
  {6,"episode_master_id","long",false,"prim",true},
  {7,"required_read_episode_master_id","long",false,"prim",false},
  {8,"theater_story_master_ids","long",true,"prim",true}
};
static const FieldSpec _k_TheaterDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"speaker","string",false,"prim",true},
  {2,"phrase","string",false,"prim",true},
  {3,"voice_asset_id","string",false,"prim",true}
};
static const FieldSpec _k_TheaterRoleMaster[] = {
  {0,"role_name","string",false,"prim",true},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"costume_master_id","long",false,"prim",false},
  {3,"order","int",false,"prim",false}
};
static const FieldSpec _k_TheaterStoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"name","string",false,"prim",true},
  {2,"order","int",false,"prim",false},
  {3,"has_synopsis","bool",false,"prim",false},
  {4,"synopsis","string",false,"prim",true},
  {5,"display_start_at","DateTime",false,"prim",false},
  {6,"display_end_at","DateTime",false,"prim",false},
  {7,"roles","TheaterRoleMaster",true,"model",true},
  {8,"details","TheaterDetailMaster",true,"model",true}
};
static const FieldSpec _k_TimeLimitedControlMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_TipMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"character_base_master_id","long",false,"prim",false},
  {2,"description","string",false,"prim",true},
  {3,"name","string",false,"prim",true}
};
static const FieldSpec _k_TitleBackgroundDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"path_type","TitleBackgroundPathTypes",false,"enum",false},
  {2,"path_value","string",false,"prim",true},
  {3,"is_title_hide","bool",false,"prim",false},
  {4,"is_decoration_hide","bool",false,"prim",false}
};
static const FieldSpec _k_TitleBackgroundMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"priority","TitleBackgroundPriorityTypes",false,"enum",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false},
  {4,"details","TitleBackgroundDetailMaster",true,"model",true}
};
static const FieldSpec _k_TitleCallVoiceMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"priority","TitleCallVoicePriorityTypes",false,"enum",false},
  {2,"voice_id","long",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",false},
  {4,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_TitleDecorationMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"decoration_type","TitleDecorationTypes",false,"enum",false},
  {2,"start_date","DateTime",false,"prim",false},
  {3,"end_date","DateTime",false,"prim",false}
};
static const FieldSpec _k_TotalPointEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"special_event_master_id","long",false,"prim",false}
};
static const FieldSpec _k_TotalPointEventRewardMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"total_point_event_master_id","long",false,"prim",false},
  {2,"point","long",false,"prim",false},
  {3,"thing_type","ThingTypes",false,"enum",false},
  {4,"thing_id","long",false,"prim",false},
  {5,"thing_quantity","int",false,"prim",false},
  {6,"order","int",false,"prim",false}
};
static const FieldSpec _k_TournamentDetailMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"difficulty","MusicDifficulties",false,"enum",false}
};
static const FieldSpec _k_TournamentMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"start_date","DateTime",false,"prim",false},
  {2,"end_date","DateTime",false,"prim",false},
  {3,"details","TournamentDetailMaster",true,"model",true}
};
static const FieldSpec _k_TournamentQualifyingMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"event_master_id","long",false,"prim",false},
  {2,"entry_page_url","string",false,"prim",true},
  {3,"entry_start_date","DateTime",false,"prim",false},
  {4,"entry_end_date","DateTime",false,"prim",false},
  {5,"website_url","string",false,"prim",true}
};
static const FieldSpec _k_TrialPartyAccessoryMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_master_id","long",false,"prim",false},
  {2,"accessory_master_id","long",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"accessory_effects","long",true,"prim",true}
};
static const FieldSpec _k_TrialPartyCharacterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_master_id","long",false,"prim",false},
  {2,"character_master_id","long",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"talent_stage","int",false,"prim",false},
  {5,"awakening_status","int",false,"prim",false},
  {6,"sense_level","int",false,"prim",false},
  {7,"read_episode_order","CharacterEpisodeOrder",false,"enum",false}
};
static const FieldSpec _k_TrialPartyEventMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"m_event_id","long",false,"prim",false},
  {2,"is_hide_complate","bool",false,"prim",false},
  {3,"start_date","DateTime",false,"prim",false},
  {4,"end_date","DateTime",false,"prim",false},
  {5,"bgm_path","string",false,"prim",true},
  {6,"name","string",false,"prim",true}
};
static const FieldSpec _k_TrialPartyEventStageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_event_master_id","long",false,"prim",false},
  {2,"music_master_id","long",false,"prim",false},
  {3,"vocal_version","int",false,"prim",false},
  {4,"sense_notation_master_id","long",false,"prim",false},
  {5,"clear_score","long",false,"prim",false},
  {6,"clear_sense_count","int",false,"prim",false},
  {7,"clear_star_act_count","int",false,"prim",false},
  {8,"clear_principal_gauge_value","int",false,"prim",false},
  {9,"title","string",false,"prim",true},
  {10,"hint","string",false,"prim",true},
  {11,"restriction_info","string",false,"prim",true},
  {12,"rewards","TrialPartyEventStageRewardMaster",true,"model",true},
  {13,"trial_party_master_id","long",false,"prim",false},
  {14,"unrecommended_auto","bool",false,"prim",false}
};
static const FieldSpec _k_TrialPartyEventStageRewardMaster[] = {
  {0,"thing_type","ThingTypes",false,"enum",false},
  {1,"thing_id","long",false,"prim",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_TrialPartyMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"leader_position","int",false,"prim",false},
  {2,"default_slots","TrialPartySlotMaster",true,"model",true}
};
static const FieldSpec _k_TrialPartyPosterMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"trial_party_master_id","long",false,"prim",false},
  {2,"poster_master_id","long",false,"prim",false},
  {3,"level","int",false,"prim",false},
  {4,"breakthrough_phase","int",false,"prim",false}
};
static const FieldSpec _k_TrialPartySlotMaster[] = {
  {0,"position","int",false,"prim",false},
  {1,"is_position_lock","bool",false,"prim",false},
  {2,"trial_party_character_master_id","long",false,"prim",false},
  {3,"character_lock_type","TrialPartyCharacterLockTypes",false,"enum",false},
  {4,"trial_party_poster_master_id","long",false,"prim",true},
  {5,"poster_lockype","TrialPartyEquipmentLockTypes",false,"enum",false},
  {6,"trial_party_accessory_master_id","long",false,"prim",true},
  {7,"accessory_lock_type","TrialPartyEquipmentLockTypes",false,"enum",false}
};
static const FieldSpec _k_TripleCastAllClassGlobalRankingRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastGroupRankingRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"rewards","TripleCastGroupRankingRewardThingMaster",true,"model",true},
  {2,"class_type","LeagueClassTypes",false,"enum",false},
  {3,"min_rank","int",false,"prim",false},
  {4,"max_rank","int",false,"prim",false},
  {5,"reward_title","string",false,"prim",true}
};
static const FieldSpec _k_TripleCastGroupRankingRewardThingMaster[] = {
  {0,"thing_id","long",false,"prim",false},
  {1,"thing_type","ThingTypes",false,"enum",false},
  {2,"thing_quantity","int",false,"prim",false}
};
static const FieldSpec _k_TripleCastMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"music_master_id","long",false,"prim",false},
  {2,"vocal_version","int",false,"prim",false},
  {3,"max_difficulty","MusicDifficulties",false,"enum",false},
  {4,"sense_notation_master_id1","long",false,"prim",false},
  {5,"sense_notation_master_id2","long",false,"prim",false},
  {6,"sense_notation_master_id3","long",false,"prim",false},
  {7,"display_start_at","DateTime",false,"prim",false},
  {8,"counting_start_at","DateTime",false,"prim",false},
  {9,"display_end_at","DateTime",false,"prim",false},
  {10,"league_class_group_master_id","long",false,"prim",false},
  {11,"triple_cast_season_master_id","long",false,"prim",false},
  {12,"vocal_effect_master_id","long",false,"prim",false},
  {13,"expression_effect_master_id","long",false,"prim",false},
  {14,"concentration_effect_master_id","long",false,"prim",false}
};
static const FieldSpec _k_UnlockConditionMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"condition_text","string",false,"prim",true},
  {2,"condition_usable_table_filter","ConditionUsableTableFilter",false,"enum",false},
  {3,"unlock_condition_type","UnlockConditionTypes",false,"enum",false}
};
static const FieldSpec _k_LeagueAllClassGlobalRankingRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"league_master_id","long",false,"prim",false},
  {2,"rewards","LeagueAllClassGlobalRankingRewardThingMaster",true,"model",true},
  {3,"min_rank","int",false,"prim",false},
  {4,"max_rank","int",false,"prim",false},
  {5,"reward_title","string",false,"prim",true}
};
static const FieldSpec _k_TripleCastAllClassGlobalRankingRewardPackageMaster[] = {
  {0,"id_","long",false,"prim",false},
  {1,"triple_cast_master_id","long",false,"prim",false},
  {2,"rewards","TripleCastAllClassGlobalRankingRewardThingMaster",true,"model",true},
  {3,"min_rank","int",false,"prim",false},
  {4,"max_rank","int",false,"prim",false},
  {5,"reward_title","string",false,"prim",true}
};
static const ModelSpec KEYS_TABLE[] = {
  {"Actor", _k_Actor, 13},
  {"LiveStatus", _k_LiveStatus, 4},
  {"Fault", _k_Fault, 3},
  {"DeletedDataObject", _k_DeletedDataObject, 2},
  {"AbilityVarietyUpPayload", _k_AbilityVarietyUpPayload, 3},
  {"AcceptFriendRequest", _k_AcceptFriendRequest, 3},
  {"Accessory", _k_Accessory, 7},
  {"AccessoryAutoSell", _k_AccessoryAutoSell, 2},
  {"AccessoryAutoSellConvertThing", _k_AccessoryAutoSellConvertThing, 2},
  {"AccessoryEffectMaster", _k_AccessoryEffectMaster, 5},
  {"AccessoryFavoritePayload", _k_AccessoryFavoritePayload, 2},
  {"AccessoryLevelPatternGroupMaster", _k_AccessoryLevelPatternGroupMaster, 2},
  {"AccessoryLevelPatternItemMaster", _k_AccessoryLevelPatternItemMaster, 2},
  {"AccessoryLevelPatternMaster", _k_AccessoryLevelPatternMaster, 5},
  {"AccessoryMaster", _k_AccessoryMaster, 10},
  {"AccountConnectPayload", _k_AccountConnectPayload, 2},
  {"AccountDeletionResult", _k_AccountDeletionResult, 2},
  {"AccountRegistResult", _k_AccountRegistResult, 2},
  {"AchivementRateRewardMaster", _k_AchivementRateRewardMaster, 5},
  {"AcquirableThing", _k_AcquirableThing, 2},
  {"AcquirableThingsPayload", _k_AcquirableThingsPayload, 1},
  {"ActorPortalCharacterPayload", _k_ActorPortalCharacterPayload, 3},
  {"Album", _k_Album, 4},
  {"AlbumArrangingPayload", _k_AlbumArrangingPayload, 4},
  {"AlbumPage", _k_AlbumPage, 6},
  {"AlbumPageResult", _k_AlbumPageResult, 8},
  {"AlbumPageSearchResult", _k_AlbumPageSearchResult, 2},
  {"AlbumPhotoPayload", _k_AlbumPhotoPayload, 2},
  {"AlbumPreset", _k_AlbumPreset, 3},
  {"AlbumTheme", _k_AlbumTheme, 2},
  {"AnotherNotation", _k_AnotherNotation, 5},
  {"AuditionClear", _k_AuditionClear, 6},
  {"AuditionClearParty", _k_AuditionClearParty, 8},
  {"AuditionClearPartySlot", _k_AuditionClearPartySlot, 15},
  {"AuditionClearedInformationParties", _k_AuditionClearedInformationParties, 2},
  {"AuditionClearedInformationResult", _k_AuditionClearedInformationResult, 1},
  {"AuditionMaster", _k_AuditionMaster, 11},
  {"AuditionPhaseMaster", _k_AuditionPhaseMaster, 7},
  {"AuditionRewardPackageMaster", _k_AuditionRewardPackageMaster, 2},
  {"AuditionRewardThing", _k_AuditionRewardThing, 3},
  {"AuthenticatePayload", _k_AuthenticatePayload, 5},
  {"AuthenticateResult", _k_AuthenticateResult, 3},
  {"BannerPayload", _k_BannerPayload, 9},
  {"BaseScoreBlock", _k_BaseScoreBlock, 6},
  {"Behaviour", _k_Behaviour, 0},
  {"BlockListResult", _k_BlockListResult, 1},
  {"Bomb", _k_Bomb, 2},
  {"BombMaster", _k_BombMaster, 6},
  {"BonusLive", _k_BonusLive, 5},
  {"BonusLiveResult", _k_BonusLiveResult, 1},
  {"BonusLiveStage", _k_BonusLiveStage, 3},
  {"BooleanResult", _k_BooleanResult, 1},
  {"BranchMaster", _k_BranchMaster, 7},
  {"BuffItemStatus", _k_BuffItemStatus, 4},
  {"BulkLevelUpPayload", _k_BulkLevelUpPayload, 2},
  {"BulkReceivePayload", _k_BulkReceivePayload, 1},
  {"CalculateLessonTimeEventPayload", _k_CalculateLessonTimeEventPayload, 2},
  {"CalculateTimeEventPayload", _k_CalculateTimeEventPayload, 5},
  {"CampaignMaster", _k_CampaignMaster, 10},
  {"ChangeNamePayload", _k_ChangeNamePayload, 1},
  {"ChangePhotoAbilityPayload", _k_ChangePhotoAbilityPayload, 3},
  {"Character", _k_Character, 15},
  {"CharacterAwakeningItemMaster", _k_CharacterAwakeningItemMaster, 4},
  {"CharacterBase", _k_CharacterBase, 8},
  {"CharacterBaseMaster", _k_CharacterBaseMaster, 26},
  {"CharacterBaseStarPointResult", _k_CharacterBaseStarPointResult, 3},
  {"CharacterBloomBonusGroupMaster", _k_CharacterBloomBonusGroupMaster, 3},
  {"CharacterBloomBonusMaster", _k_CharacterBloomBonusMaster, 5},
  {"CharacterBloomItemMaster", _k_CharacterBloomItemMaster, 7},
  {"CharacterBloomRewardMaster", _k_CharacterBloomRewardMaster, 4},
  {"CharacterCategoryMaster", _k_CharacterCategoryMaster, 2},
  {"CharacterExperienceItemMaster", _k_CharacterExperienceItemMaster, 4},
  {"CharacterFavoritePayload", _k_CharacterFavoritePayload, 2},
  {"CharacterLesson", _k_CharacterLesson, 5},
  {"CharacterLessonSlot", _k_CharacterLessonSlot, 2},
  {"CharacterLevelMaster", _k_CharacterLevelMaster, 4},
  {"CharacterLineupResult", _k_CharacterLineupResult, 4},
  {"CharacterMaster", _k_CharacterMaster, 27},
  {"CharacterMission", _k_CharacterMission, 8},
  {"CharacterMissionMaster", _k_CharacterMissionMaster, 5},
  {"CharacterMissionStageMaster", _k_CharacterMissionStageMaster, 7},
  {"CharacterPieceMaster", _k_CharacterPieceMaster, 4},
  {"CharacterPointEvent", _k_CharacterPointEvent, 6},
  {"CharacterPointEventInformationResult", _k_CharacterPointEventInformationResult, 3},
  {"CharacterPointEventRankingResult", _k_CharacterPointEventRankingResult, 2},
  {"CharacterRank", _k_CharacterRank, 2},
  {"CharacterRarityProbability", _k_CharacterRarityProbability, 2},
  {"CharacterSenseEnhanceItemGroupMaster", _k_CharacterSenseEnhanceItemGroupMaster, 2},
  {"CharacterSenseEnhanceItemMaster", _k_CharacterSenseEnhanceItemMaster, 3},
  {"CharacterStarRankMaster", _k_CharacterStarRankMaster, 4},
  {"CharacterStarRankRewardGroupMaster", _k_CharacterStarRankRewardGroupMaster, 2},
  {"CharacterStarRankRewardMaster", _k_CharacterStarRankRewardMaster, 3},
  {"ChatworkSendMessagePayload", _k_ChatworkSendMessagePayload, 1},
  {"CircleAuthorityChangePayload", _k_CircleAuthorityChangePayload, 3},
  {"CircleAuthorityResult", _k_CircleAuthorityResult, 1},
  {"CircleBanner", _k_CircleBanner, 8},
  {"CircleEventCircleMissionProgress", _k_CircleEventCircleMissionProgress, 2},
  {"CircleEventInformationResult", _k_CircleEventInformationResult, 5},
  {"CircleEventMission", _k_CircleEventMission, 3},
  {"CircleEventRanking", _k_CircleEventRanking, 2},
  {"CircleInformationResult", _k_CircleInformationResult, 15},
  {"CircleInviteResult", _k_CircleInviteResult, 2},
  {"CircleMemberInfoParameters", _k_CircleMemberInfoParameters, 17},
  {"CircleMemberInfoResult", _k_CircleMemberInfoResult, 3},
  {"CircleMemberParameters", _k_CircleMemberParameters, 14},
  {"CirclePayload", _k_CirclePayload, 9},
  {"CircleProfile", _k_CircleProfile, 6},
  {"CircleRawRanking", _k_CircleRawRanking, 3},
  {"CircleResult", _k_CircleResult, 1},
  {"CircleSearchIdResult", _k_CircleSearchIdResult, 2},
  {"CircleSearchResult", _k_CircleSearchResult, 3},
  {"ComebackCampaign", _k_ComebackCampaign, 2},
  {"Comic", _k_Comic, 1},
  {"CompanyMaster", _k_CompanyMaster, 5},
  {"Component", _k_Component, 0},
  {"ConcertResult", _k_ConcertResult, 1},
  {"ConcertStage", _k_ConcertStage, 1},
  {"ConcoursDetailInformationResult", _k_ConcoursDetailInformationResult, 2},
  {"ConcoursInfomationResult", _k_ConcoursInfomationResult, 2},
  {"ConnectWithAccount", _k_ConnectWithAccount, 2},
  {"ConnectWithPassword", _k_ConnectWithPassword, 1},
  {"ConvertedThingResult", _k_ConvertedThingResult, 3},
  {"Costume", _k_Costume, 2},
  {"CostumeFavoritePayload", _k_CostumeFavoritePayload, 3},
  {"CostumeMaster", _k_CostumeMaster, 6},
  {"CostumeWearableCharacterGroupMaster", _k_CostumeWearableCharacterGroupMaster, 2},
  {"CourseRankingResult", _k_CourseRankingResult, 5},
  {"CourseResult", _k_CourseResult, 3},
  {"CreateCircleResult", _k_CreateCircleResult, 2},
  {"CreateMultiRoomPayload", _k_CreateMultiRoomPayload, 9},
  {"Currency", _k_Currency, 4},
  {"DailyLesson", _k_DailyLesson, 1},
  {"DailyLimit", _k_DailyLimit, 5},
  {"DebugAlbumSimpleArrangingPayload", _k_DebugAlbumSimpleArrangingPayload, 4},
  {"DebugEditLeagueBasicPayload", _k_DebugEditLeagueBasicPayload, 2},
  {"DebugLinkageCodeResult", _k_DebugLinkageCodeResult, 2},
  {"DebugModifyCharacterEnhanceInformationPayload", _k_DebugModifyCharacterEnhanceInformationPayload, 5},
  {"DebugModifyPosterEnhanceInformationPayload", _k_DebugModifyPosterEnhanceInformationPayload, 3},
  {"DebugPrepareLeagueGroupUserPayload", _k_DebugPrepareLeagueGroupUserPayload, 3},
  {"DebugUserIdResult", _k_DebugUserIdResult, 2},
  {"Decimal", _k_Decimal, 0},
  {"Decoration", _k_Decoration, 2},
  {"Dictionary", _k_Dictionary, 0},
  {"DonateSupportCompanyResult", _k_DonateSupportCompanyResult, 2},
  {"DonateSupportLevelLimitDetail", _k_DonateSupportLevelLimitDetail, 2},
  {"DonateSupportLevelLimitPayload", _k_DonateSupportLevelLimitPayload, 4},
  {"DugongRun", _k_DugongRun, 4},
  {"EditBookmarkPayload", _k_EditBookmarkPayload, 2},
  {"EditPartyPayload", _k_EditPartyPayload, 6},
  {"EditPartySlotPayload", _k_EditPartySlotPayload, 5},
  {"EditPositionPayload", _k_EditPositionPayload, 1},
  {"EditPositionSlotPayload", _k_EditPositionSlotPayload, 2},
  {"EditTrialPartyEventStagePartyPayload", _k_EditTrialPartyEventStagePartyPayload, 3},
  {"EditTrialPartyEventStagePartyPayloadSlot", _k_EditTrialPartyEventStagePartyPayloadSlot, 4},
  {"EditUserProfilePayload", _k_EditUserProfilePayload, 14},
  {"EexternalPaymentResult", _k_EexternalPaymentResult, 1},
  {"Effect", _k_Effect, 6},
  {"EffectBranch", _k_EffectBranch, 3},
  {"EffectConditionMaster", _k_EffectConditionMaster, 2},
  {"EffectDetailMaster", _k_EffectDetailMaster, 2},
  {"EffectDurationGroupMaster", _k_EffectDurationGroupMaster, 2},
  {"EffectDurationMaster", _k_EffectDurationMaster, 2},
  {"EffectMaster", _k_EffectMaster, 9},
  {"EffectOrderMaster", _k_EffectOrderMaster, 2},
  {"EffectTargetValue", _k_EffectTargetValue, 2},
  {"EffectTrigger", _k_EffectTrigger, 2},
  {"EffectTriggerMaster", _k_EffectTriggerMaster, 2},
  {"EnvironmentResult", _k_EnvironmentResult, 13},
  {"Episode", _k_Episode, 2},
  {"EpisodeMaster", _k_EpisodeMaster, 9},
  {"EpisodeReleaseCondition", _k_EpisodeReleaseCondition, 4},
  {"EpisodeResult", _k_EpisodeResult, 4},
  {"EpisodeDetailResult", _k_EpisodeDetailResult, 24},
  {"EpisodeDetailCharacterMotionResult", _k_EpisodeDetailCharacterMotionResult, 11},
  {"EpisodeRewardPackageMaster", _k_EpisodeRewardPackageMaster, 2},
  {"EpisodeRewardThing", _k_EpisodeRewardThing, 5},
  {"Event", _k_Event, 7},
  {"EventBoxGacha", _k_EventBoxGacha, 3},
  {"EventBoxGachaBoxThing", _k_EventBoxGachaBoxThing, 3},
  {"EventBoxGachaRollResult", _k_EventBoxGachaRollResult, 3},
  {"EventCamp", _k_EventCamp, 4},
  {"EventResult", _k_EventResult, 14},
  {"ExchangeLimit", _k_ExchangeLimit, 6},
  {"ExchangeShopMaster", _k_ExchangeShopMaster, 12},
  {"ExchangeShopThing", _k_ExchangeShopThing, 14},
  {"ExchangeShopThingPayload", _k_ExchangeShopThingPayload, 2},
  {"FavoriteCostume", _k_FavoriteCostume, 3},
  {"FavoriteStampOrderPayload", _k_FavoriteStampOrderPayload, 1},
  {"FinishAnotherNotationLivePayload", _k_FinishAnotherNotationLivePayload, 6},
  {"FinishLessonPayload", _k_FinishLessonPayload, 3},
  {"FinishLivePayload", _k_FinishLivePayload, 10},
  {"FinishLiveResult", _k_FinishLiveResult, 27},
  {"FlashSaleReadStagePayload", _k_FlashSaleReadStagePayload, 1},
  {"FlashSaleStage", _k_FlashSaleStage, 5},
  {"FriendAcceptResult", _k_FriendAcceptResult, 1},
  {"FriendFavoritePayload", _k_FriendFavoritePayload, 2},
  {"FriendInvitation", _k_FriendInvitation, 3},
  {"FriendInvitationMission", _k_FriendInvitationMission, 6},
  {"FriendInvitationMissionPayload", _k_FriendInvitationMissionPayload, 1},
  {"FriendInvitationUserInfoResult", _k_FriendInvitationUserInfoResult, 4},
  {"FriendListResult", _k_FriendListResult, 2},
  {"FriendRequest", _k_FriendRequest, 3},
  {"FriendRequestResult", _k_FriendRequestResult, 1},
  {"FriendResult", _k_FriendResult, 17},
  {"FriendSearchResult", _k_FriendSearchResult, 2},
  {"Gacha", _k_Gacha, 3},
  {"GachaHistoryResult", _k_GachaHistoryResult, 2},
  {"GachaInfoResult", _k_GachaInfoResult, 4},
  {"GachaLineupItemProbability", _k_GachaLineupItemProbability, 2},
  {"GachaReRoll", _k_GachaReRoll, 3},
  {"GachaRollLimit", _k_GachaRollLimit, 2},
  {"GachaRollResult", _k_GachaRollResult, 5},
  {"GachaSelectedThing", _k_GachaSelectedThing, 2},
  {"GachaSelectedThingsResult", _k_GachaSelectedThingsResult, 1},
  {"GachaThingResult", _k_GachaThingResult, 3},
  {"GameHint", _k_GameHint, 3},
  {"GameHintMonoBehaviour", _k_GameHintMonoBehaviour, 0},
  {"GenerateLotteryResultPayload", _k_GenerateLotteryResultPayload, 2},
  {"GeneratePhotoPayload", _k_GeneratePhotoPayload, 5},
  {"GeneratePhotoResult", _k_GeneratePhotoResult, 6},
  {"GeneratePhotosPayload", _k_GeneratePhotosPayload, 1},
  {"GhostLiveInfo", _k_GhostLiveInfo, 2},
  {"GhostLiveResult", _k_GhostLiveResult, 6},
  {"GradualMissionGroup", _k_GradualMissionGroup, 3},
  {"HighScoreBuff", _k_HighScoreBuff, 2},
  {"HighScoreBuffSetting", _k_HighScoreBuffSetting, 2},
  {"HighScoreParty", _k_HighScoreParty, 6},
  {"HighScorePartySlot", _k_HighScorePartySlot, 12},
  {"HomeBGM", _k_HomeBGM, 3},
  {"HomeCharacterVoiceMaster", _k_HomeCharacterVoiceMaster, 21},
  {"HomeDisplayPreference", _k_HomeDisplayPreference, 14},
  {"HomeSkin", _k_HomeSkin, 2},
  {"IconFrame", _k_IconFrame, 2},
  {"Inbox", _k_Inbox, 11},
  {"InboxReceiveResult", _k_InboxReceiveResult, 2},
  {"InviteMultiRoomPayload", _k_InviteMultiRoomPayload, 1},
  {"Item", _k_Item, 3},
  {"ItemMaster", _k_ItemMaster, 12},
  {"JewelShop", _k_JewelShop, 5},
  {"JoinMultiRoomPayload", _k_JoinMultiRoomPayload, 2},
  {"LeagueBasic", _k_LeagueBasic, 6},
  {"LeagueGroup", _k_LeagueGroup, 3},
  {"LeagueGroupMember", _k_LeagueGroupMember, 3},
  {"LeagueHighScoreParty", _k_LeagueHighScoreParty, 11},
  {"LeagueHighScorePartySlot", _k_LeagueHighScorePartySlot, 14},
  {"LeagueHistory", _k_LeagueHistory, 9},
  {"LeaguePartyAndRankingResult", _k_LeaguePartyAndRankingResult, 9},
  {"LeaguePartySlotResult", _k_LeaguePartySlotResult, 17},
  {"LeagueReceiveResults", _k_LeagueReceiveResults, 6},
  {"LeagueResult", _k_LeagueResult, 6},
  {"LeagueSeasonResult", _k_LeagueSeasonResult, 2},
  {"LeagueTopMenuInformationResult", _k_LeagueTopMenuInformationResult, 12},
  {"LessonResult", _k_LessonResult, 5},
  {"LevelUpPhotoPayload", _k_LevelUpPhotoPayload, 2},
  {"LevelUpPhotoResult", _k_LevelUpPhotoResult, 3},
  {"Limit", _k_Limit, 5},
  {"LinkCharacter", _k_LinkCharacter, 5},
  {"Live", _k_Live, 8},
  {"LiveAchievement", _k_LiveAchievement, 3},
  {"LiveDropCelling", _k_LiveDropCelling, 4},
  {"LiveDropLimit", _k_LiveDropLimit, 3},
  {"LiveDropThing", _k_LiveDropThing, 4},
  {"LiveMaster", _k_LiveMaster, 9},
  {"LiveSettingMaster", _k_LiveSettingMaster, 4},
  {"LiveTimeEvent", _k_LiveTimeEvent, 2},
  {"LiveUnit", _k_LiveUnit, 12},
  {"LiveUnitWithOrder", _k_LiveUnitWithOrder, 13},
  {"LoginBonusDetail", _k_LoginBonusDetail, 4},
  {"LoginBonusResult", _k_LoginBonusResult, 25},
  {"LoginBonusSpineDetail", _k_LoginBonusSpineDetail, 10},
  {"LoginBonusSpineGroup", _k_LoginBonusSpineGroup, 2},
  {"LoginPassStatus", _k_LoginPassStatus, 2},
  {"LoginPayload", _k_LoginPayload, 1},
  {"LoginResult", _k_LoginResult, 7},
  {"Lottery", _k_Lottery, 2},
  {"LotteryPriseResult", _k_LotteryPriseResult, 2},
  {"MainCharacter", _k_MainCharacter, 5},
  {"Market", _k_Market, 3},
  {"MarketResult", _k_MarketResult, 2},
  {"MarketThing", _k_MarketThing, 4},
  {"MarshalByRefObject", _k_MarshalByRefObject, 0},
  {"MasterDataManifest", _k_MasterDataManifest, 4},
  {"MatchingGhostLiveResult", _k_MatchingGhostLiveResult, 3},
  {"Mission", _k_Mission, 6},
  {"MissionCleared", _k_MissionCleared, 2},
  {"MissionMaster", _k_MissionMaster, 12},
  {"MissionPass", _k_MissionPass, 11},
  {"MissionPassDetailMaster", _k_MissionPassDetailMaster, 7},
  {"MissionPassMaster", _k_MissionPassMaster, 4},
  {"MissionPassRewardThing", _k_MissionPassRewardThing, 6},
  {"MissionPassRewardsResult", _k_MissionPassRewardsResult, 2},
  {"MissionRewardMaster", _k_MissionRewardMaster, 3},
  {"MissionStageMaster", _k_MissionStageMaster, 5},
  {"MonoBehaviour", _k_MonoBehaviour, 0},
  {"MultiLiveAdditionalScoreBlock", _k_MultiLiveAdditionalScoreBlock, 5},
  {"MultiLiveInformation", _k_MultiLiveInformation, 1},
  {"MultiLiveRestriction", _k_MultiLiveRestriction, 1},
  {"MultiRoomBasic", _k_MultiRoomBasic, 2},
  {"MultiRoomCreateResult", _k_MultiRoomCreateResult, 2},
  {"MultiRoomDetailResult", _k_MultiRoomDetailResult, 4},
  {"MultiRoomInformationResult", _k_MultiRoomInformationResult, 11},
  {"MultiRoomInvitedResult", _k_MultiRoomInvitedResult, 2},
  {"MultiRoomJoinnedResult", _k_MultiRoomJoinnedResult, 4},
  {"MultiRoomPartySlot", _k_MultiRoomPartySlot, 13},
  {"MultiRoomRanking", _k_MultiRoomRanking, 10},
  {"Music", _k_Music, 6},
  {"MusicBookmark", _k_MusicBookmark, 2},
  {"MusicCourse", _k_MusicCourse, 5},
  {"MusicCourseRandomSelectResult", _k_MusicCourseRandomSelectResult, 1},
  {"MusicCourseRandomSelectResultDetail", _k_MusicCourseRandomSelectResultDetail, 2},
  {"MusicCourseRanking", _k_MusicCourseRanking, 13},
  {"MusicCourseRankingPayload", _k_MusicCourseRankingPayload, 11},
  {"MusicMaster", _k_MusicMaster, 25},
  {"MusicVideo", _k_MusicVideo, 2},
  {"MusicVocalVersionMaster", _k_MusicVocalVersionMaster, 10},
  {"MyCircleInformationResult", _k_MyCircleInformationResult, 18},
  {"NameBaseColor", _k_NameBaseColor, 2},
  {"NameColor", _k_NameColor, 2},
  {"NameColorMaster", _k_NameColorMaster, 7},
  {"Nameplate", _k_Nameplate, 2},
  {"NameplateDetailMaster", _k_NameplateDetailMaster, 7},
  {"NameplateMaster", _k_NameplateMaster, 11},
  {"Note", _k_Note, 2},
  {"NoteJudgePayload", _k_NoteJudgePayload, 2},
  {"NoteMaster", _k_NoteMaster, 6},
  {"NoteResult", _k_NoteResult, 6},
  {"Notification", _k_Notification, 4},
  {"NotificationContentResult", _k_NotificationContentResult, 8},
  {"NotificationResult", _k_NotificationResult, 9},
  {"Party", _k_Party, 4},
  {"PartyInfo", _k_PartyInfo, 2},
  {"PartyPayload", _k_PartyPayload, 2},
  {"PartySlot", _k_PartySlot, 7},
  {"PartySlotAccessoryPayload", _k_PartySlotAccessoryPayload, 5},
  {"PartySlotCharacterPayload", _k_PartySlotCharacterPayload, 9},
  {"PartySlotDetail", _k_PartySlotDetail, 15},
  {"PartySlotPayload", _k_PartySlotPayload, 4},
  {"PartySlotPosterPayload", _k_PartySlotPosterPayload, 4},
  {"PermanentMarketThing", _k_PermanentMarketThing, 2},
  {"Photo", _k_Photo, 14},
  {"PhotoAppearedCharacter", _k_PhotoAppearedCharacter, 2},
  {"PickupCharacterMission", _k_PickupCharacterMission, 2},
  {"PlayerRankPointResult", _k_PlayerRankPointResult, 6},
  {"Poster", _k_Poster, 8},
  {"PosterAlternativeImagePayload", _k_PosterAlternativeImagePayload, 2},
  {"PosterCostumeMaster", _k_PosterCostumeMaster, 5},
  {"PosterFavoritePayload", _k_PosterFavoritePayload, 2},
  {"PosterLevelPatternGroupMaster", _k_PosterLevelPatternGroupMaster, 2},
  {"PosterLevelPatternMaster", _k_PosterLevelPatternMaster, 5},
  {"PosterLineupResult", _k_PosterLineupResult, 4},
  {"PosterMaster", _k_PosterMaster, 35},
  {"PosterRarityProbability", _k_PosterRarityProbability, 2},
  {"PosterReleaseItemGroupMaster", _k_PosterReleaseItemGroupMaster, 3},
  {"PosterReleaseItemMaster", _k_PosterReleaseItemMaster, 3},
  {"PosterStoryMaster", _k_PosterStoryMaster, 8},
  {"ProcessPaymentResult", _k_ProcessPaymentResult, 1},
  {"PurchaseItemPayload", _k_PurchaseItemPayload, 1},
  {"RandomEffectGroupMaster", _k_RandomEffectGroupMaster, 2},
  {"RateResult", _k_RateResult, 4},
  {"RateUpdateResult", _k_RateUpdateResult, 2},
  {"RawHighScoreRankingResult", _k_RawHighScoreRankingResult, 4},
  {"RawRanking", _k_RawRanking, 4},
  {"RawRankingResult", _k_RawRankingResult, 2},
  {"RawRankingWithLongPoint", _k_RawRankingWithLongPoint, 3},
  {"ReadNotificationPayload", _k_ReadNotificationPayload, 2},
  {"ReceivedThing", _k_ReceivedThing, 7},
  {"RecyclableMonoBehaviour", _k_RecyclableMonoBehaviour, 0},
  {"RegisterAppStorePaymentPayload", _k_RegisterAppStorePaymentPayload, 1},
  {"RegisterBirthDayPayload", _k_RegisterBirthDayPayload, 1},
  {"RegisterGooglePlayPaymentPayload", _k_RegisterGooglePlayPaymentPayload, 1},
  {"RegisterPayload", _k_RegisterPayload, 1},
  {"RegisterTakeOverPasswordPayload", _k_RegisterTakeOverPasswordPayload, 1},
  {"Restriction", _k_Restriction, 3},
  {"RewardRuleMaster", _k_RewardRuleMaster, 2},
  {"RollResult", _k_RollResult, 3},
  {"Roulette", _k_Roulette, 3},
  {"RouletteEvent", _k_RouletteEvent, 3},
  {"RouletteRollResult", _k_RouletteRollResult, 1},
  {"ScoreWithDateResult", _k_ScoreWithDateResult, 2},
  {"SelectMusicCourseRandomMusicPayload", _k_SelectMusicCourseRandomMusicPayload, 2},
  {"SellAccessoryPayload", _k_SellAccessoryPayload, 1},
  {"Sense", _k_Sense, 12},
  {"SenseCoolTime", _k_SenseCoolTime, 2},
  {"SenseEffect", _k_SenseEffect, 4},
  {"SenseEffectMaster", _k_SenseEffectMaster, 2},
  {"SenseMaster", _k_SenseMaster, 16},
  {"SenseScoreBlock", _k_SenseScoreBlock, 6},
  {"SenseTimingEvent", _k_SenseTimingEvent, 3},
  {"SetAlbumPublishingPayload", _k_SetAlbumPublishingPayload, 2},
  {"SetLessonPartyPayload", _k_SetLessonPartyPayload, 1},
  {"SetLessonPartySlotPayload", _k_SetLessonPartySlotPayload, 2},
  {"SetPhotoTagPayload", _k_SetPhotoTagPayload, 2},
  {"SetSelectedThingsPayload", _k_SetSelectedThingsPayload, 1},
  {"SpRate", _k_SpRate, 3},
  {"SpRateUpdateResult", _k_SpRateUpdateResult, 4},
  {"SpecialEvent", _k_SpecialEvent, 3},
  {"SpotConversationMaster", _k_SpotConversationMaster, 14},
  {"Stamp", _k_Stamp, 3},
  {"StampMaster", _k_StampMaster, 9},
  {"StarAct", _k_StarAct, 4},
  {"StarActScoreBlock", _k_StarActScoreBlock, 5},
  {"StarPassStatus", _k_StarPassStatus, 4},
  {"StarPointResult", _k_StarPointResult, 6},
  {"StarRankRewardMaster", _k_StarRankRewardMaster, 3},
  {"StartLessonPayload", _k_StartLessonPayload, 2},
  {"StartLivePayload", _k_StartLivePayload, 15},
  {"StartMultiLivePayload", _k_StartMultiLivePayload, 5},
  {"StartMultiRoomLivePayload", _k_StartMultiRoomLivePayload, 3},
  {"StartTournamentPayload", _k_StartTournamentPayload, 3},
  {"StartTripleCastLivePayload", _k_StartTripleCastLivePayload, 2},
  {"StartTripleCastLiveResult", _k_StartTripleCastLiveResult, 1},
  {"Status", _k_Status, 3},
  {"StoryEvent", _k_StoryEvent, 7},
  {"StoryEventCampInfo", _k_StoryEventCampInfo, 3},
  {"StoryEventCircle", _k_StoryEventCircle, 5},
  {"StoryEventCircleMission", _k_StoryEventCircleMission, 3},
  {"StoryEventCircleMissionReward", _k_StoryEventCircleMissionReward, 3},
  {"StoryEventHighScore", _k_StoryEventHighScore, 4},
  {"StoryEventHighScoreBuffSetting", _k_StoryEventHighScoreBuffSetting, 2},
  {"StoryEventHighScoreParty", _k_StoryEventHighScoreParty, 9},
  {"StoryEventHighScorePartySlot", _k_StoryEventHighScorePartySlot, 14},
  {"StoryEventMissionCircleProgress", _k_StoryEventMissionCircleProgress, 2},
  {"StoryEventMissionCircleProgressResult", _k_StoryEventMissionCircleProgressResult, 2},
  {"StoryEventPointExchangeResult", _k_StoryEventPointExchangeResult, 3},
  {"StoryMaster", _k_StoryMaster, 7},
  {"SupportCompanyInformation", _k_SupportCompanyInformation, 4},
  {"SupportCompanyLevelLimitDetail", _k_SupportCompanyLevelLimitDetail, 2},
  {"SupportCompanyLevelLimitPayload", _k_SupportCompanyLevelLimitPayload, 3},
  {"SupportCompanyLevelLimitStatus", _k_SupportCompanyLevelLimitStatus, 2},
  {"SupportCompanyLevelLimitStatusDetail", _k_SupportCompanyLevelLimitStatusDetail, 2},
  {"SupportCompanyLevelStatus", _k_SupportCompanyLevelStatus, 5},
  {"TakeOverAccountPayload", _k_TakeOverAccountPayload, 2},
  {"TakeOverAccountResult", _k_TakeOverAccountResult, 5},
  {"TakeOverCodeResult", _k_TakeOverCodeResult, 2},
  {"TheaterStory", _k_TheaterStory, 2},
  {"TimeLimitedControl", _k_TimeLimitedControl, 3},
  {"TimedConfirmationCode", _k_TimedConfirmationCode, 2},
  {"TimingEvent", _k_TimingEvent, 8},
  {"TotalPointEvent", _k_TotalPointEvent, 4},
  {"TotalPointEventInformationResult", _k_TotalPointEventInformationResult, 1},
  {"TotalPointEventRankingResult", _k_TotalPointEventRankingResult, 2},
  {"TournamentDetail", _k_TournamentDetail, 10},
  {"TournamentQualifying", _k_TournamentQualifying, 13},
  {"TournamentQualifyingInformationResult", _k_TournamentQualifyingInformationResult, 1},
  {"TournamentResult", _k_TournamentResult, 3},
  {"TransitionTokenResult", _k_TransitionTokenResult, 1},
  {"TrialPartyEvent", _k_TrialPartyEvent, 4},
  {"TrialPartyEventResult", _k_TrialPartyEventResult, 1},
  {"TrialPartyEventStage", _k_TrialPartyEventStage, 3},
  {"TrialPartyEventStageParty", _k_TrialPartyEventStageParty, 3},
  {"TrialPartyEventStagePartySlot", _k_TrialPartyEventStagePartySlot, 6},
  {"TrialPartyEventStageResult", _k_TrialPartyEventStageResult, 1},
  {"TripleCastBasic", _k_TripleCastBasic, 9},
  {"TripleCastGroup", _k_TripleCastGroup, 3},
  {"TripleCastGroupMember", _k_TripleCastGroupMember, 3},
  {"TripleCastHighScoreParty", _k_TripleCastHighScoreParty, 8},
  {"TripleCastHighScorePartySlot", _k_TripleCastHighScorePartySlot, 14},
  {"TripleCastHistory", _k_TripleCastHistory, 9},
  {"TripleCastParty", _k_TripleCastParty, 4},
  {"TripleCastPartyAndRankingResult", _k_TripleCastPartyAndRankingResult, 8},
  {"TripleCastPartyResult", _k_TripleCastPartyResult, 3},
  {"TripleCastPartyScore", _k_TripleCastPartyScore, 2},
  {"TripleCastPartySlot", _k_TripleCastPartySlot, 7},
  {"TripleCastSeasonResult", _k_TripleCastSeasonResult, 2},
  {"Trophy", _k_Trophy, 4},
  {"TrophyGroupMaster", _k_TrophyGroupMaster, 2},
  {"TrophyMaster", _k_TrophyMaster, 8},
  {"UpdateClearLampResult", _k_UpdateClearLampResult, 1},
  {"UpdateGameHintPayload", _k_UpdateGameHintPayload, 1},
  {"UpdateHomeDisplayPreferencePayload", _k_UpdateHomeDisplayPreferencePayload, 13},
  {"UpdateLastViewedAtPayload", _k_UpdateLastViewedAtPayload, 2},
  {"UpdateTutorialPayload", _k_UpdateTutorialPayload, 1},
  {"UrlResult", _k_UrlResult, 1},
  {"UseExperienceItemsPayload", _k_UseExperienceItemsPayload, 2},
  {"UseStaminaRecoveryItem", _k_UseStaminaRecoveryItem, 2},
  {"UseStaminaRecoveryItemsPayload", _k_UseStaminaRecoveryItemsPayload, 1},
  {"User", _k_User, 20},
  {"UserBlock", _k_UserBlock, 2},
  {"UserBonus", _k_UserBonus, 3},
  {"UserPreference", _k_UserPreference, 3},
  {"UserProfile", _k_UserProfile, 21},
  {"UserProfileDetail", _k_UserProfileDetail, 13},
  {"UserResult", _k_UserResult, 1},
  {"ViewShopResult", _k_ViewShopResult, 1},
  {"ViewedShop", _k_ViewedShop, 4},
  {"AccessoryEffectFilterMaster", _k_AccessoryEffectFilterMaster, 5},
  {"ActivityLogMessageTemplateMaster", _k_ActivityLogMessageTemplateMaster, 3},
  {"AdditionalRewardPackageMaster", _k_AdditionalRewardPackageMaster, 8},
  {"AdditionalRewardThingMaster", _k_AdditionalRewardThingMaster, 3},
  {"AlbumEffectMaster", _k_AlbumEffectMaster, 4},
  {"AlbumThemeMaster", _k_AlbumThemeMaster, 8},
  {"AnotherNotationMaster", _k_AnotherNotationMaster, 9},
  {"BannerMaster", _k_BannerMaster, 12},
  {"BodyMotionMaster", _k_BodyMotionMaster, 2},
  {"BonusLiveMaster", _k_BonusLiveMaster, 7},
  {"BonusLiveStageMaster", _k_BonusLiveStageMaster, 13},
  {"BonusLiveStageRewardThingMaster", _k_BonusLiveStageRewardThingMaster, 5},
  {"BuffItemMaster", _k_BuffItemMaster, 7},
  {"CategoryGroupMaster", _k_CategoryGroupMaster, 4},
  {"CategoryMaster", _k_CategoryMaster, 4},
  {"ChangeBodyMotionMaster", _k_ChangeBodyMotionMaster, 4},
  {"CharacterAwakeningItemGroupMaster", _k_CharacterAwakeningItemGroupMaster, 2},
  {"CharacterBaseBloomGenericItemMaster", _k_CharacterBaseBloomGenericItemMaster, 4},
  {"CharacterBloomDetailMaster", _k_CharacterBloomDetailMaster, 3},
  {"CharacterEpisodeMaster", _k_CharacterEpisodeMaster, 5},
  {"CharacterEpisodeRelationMaster", _k_CharacterEpisodeRelationMaster, 5},
  {"CharacterEpisodeReleaseItemGroupMaster", _k_CharacterEpisodeReleaseItemGroupMaster, 2},
  {"CharacterEpisodeReleaseItemMaster", _k_CharacterEpisodeReleaseItemMaster, 3},
  {"CharacterKeyMissionMaster", _k_CharacterKeyMissionMaster, 7},
  {"CharacterLessonScoreRewardMaster", _k_CharacterLessonScoreRewardMaster, 4},
  {"CharacterMissionCategoryLevelMaster", _k_CharacterMissionCategoryLevelMaster, 6},
  {"CharacterMissionItemMaster", _k_CharacterMissionItemMaster, 4},
  {"CharacterPointEventCharacterRankingRewardItemMaster", _k_CharacterPointEventCharacterRankingRewardItemMaster, 3},
  {"CharacterPointEventCharacterRankingRewardItemPackageMaster", _k_CharacterPointEventCharacterRankingRewardItemPackageMaster, 2},
  {"CharacterPointEventCharacterRankingRewardMaster", _k_CharacterPointEventCharacterRankingRewardMaster, 6},
  {"CharacterPointEventMaster", _k_CharacterPointEventMaster, 6},
  {"CharacterProfileRestrictionMaster", _k_CharacterProfileRestrictionMaster, 5},
  {"CircleEventCirclePointRewardMaster", _k_CircleEventCirclePointRewardMaster, 7},
  {"CircleEventCircleRankingRewardItem", _k_CircleEventCircleRankingRewardItem, 3},
  {"CircleEventCircleRankingRewardItemPackageMaster", _k_CircleEventCircleRankingRewardItemPackageMaster, 2},
  {"CircleEventCircleRankingRewardMaster", _k_CircleEventCircleRankingRewardMaster, 5},
  {"CircleEventMaster", _k_CircleEventMaster, 3},
  {"CircleEventMissionMaster", _k_CircleEventMissionMaster, 11},
  {"CircleEventMissionRefreshSetting", _k_CircleEventMissionRefreshSetting, 3},
  {"CircleEventMissionRefreshSettingGroupMaster", _k_CircleEventMissionRefreshSettingGroupMaster, 2},
  {"CircleSupportCompanyLevelDetailMaster", _k_CircleSupportCompanyLevelDetailMaster, 7},
  {"CircleSupportCompanyLevelLimitDetailMaster", _k_CircleSupportCompanyLevelLimitDetailMaster, 3},
  {"CircleSupportCompanyLevelLimitMaster", _k_CircleSupportCompanyLevelLimitMaster, 5},
  {"CircleTheaterLevelMaster", _k_CircleTheaterLevelMaster, 4},
  {"ComebackCampaignMaster", _k_ComebackCampaignMaster, 5},
  {"ComicEpisodeMaster", _k_ComicEpisodeMaster, 5},
  {"ComicMaster", _k_ComicMaster, 3},
  {"ConcertMaster", _k_ConcertMaster, 5},
  {"ConcertRewardMaster", _k_ConcertRewardMaster, 3},
  {"ConcertStageMaster", _k_ConcertStageMaster, 9},
  {"ConcoursDetailMaster", _k_ConcoursDetailMaster, 4},
  {"ConcoursMaster", _k_ConcoursMaster, 5},
  {"ConcoursPointMaster", _k_ConcoursPointMaster, 3},
  {"ConcoursPointRewardMaster", _k_ConcoursPointRewardMaster, 4},
  {"ConcoursPointRewardThingMaster", _k_ConcoursPointRewardThingMaster, 3},
  {"CostumeGroupMaster", _k_CostumeGroupMaster, 10},
  {"CourseRankingRewardMaster", _k_CourseRankingRewardMaster, 5},
  {"CourseRankingRewardThingMaster", _k_CourseRankingRewardThingMaster, 4},
  {"DecorationMaster", _k_DecorationMaster, 9},
  {"DugongRunCourseMaster", _k_DugongRunCourseMaster, 7},
  {"DugongRunRewardMaster", _k_DugongRunRewardMaster, 6},
  {"EffectTriggerCharacterBaseGroupMaster", _k_EffectTriggerCharacterBaseGroupMaster, 2},
  {"EventBonusMaster", _k_EventBonusMaster, 3},
  {"EventBoxGachaBoxMaster", _k_EventBoxGachaBoxMaster, 4},
  {"EventBoxGachaBoxThingMaster", _k_EventBoxGachaBoxThingMaster, 6},
  {"EventBoxGachaDetailMaster", _k_EventBoxGachaDetailMaster, 3},
  {"EventBoxGachaMaster", _k_EventBoxGachaMaster, 7},
  {"EventBoxGachaTextTemplateMaster", _k_EventBoxGachaTextTemplateMaster, 2},
  {"EventCampClassMaster", _k_EventCampClassMaster, 5},
  {"EventCampMaster", _k_EventCampMaster, 11},
  {"EventCampSupportPointRewardMaster", _k_EventCampSupportPointRewardMaster, 6},
  {"EventMaster", _k_EventMaster, 17},
  {"EventPointRewardMaster", _k_EventPointRewardMaster, 5},
  {"EventRankingRewardMaster", _k_EventRankingRewardMaster, 4},
  {"EventRankingRewardThingMaster", _k_EventRankingRewardThingMaster, 5},
  {"FacialExpressionMaster", _k_FacialExpressionMaster, 6},
  {"FilmItemMaster", _k_FilmItemMaster, 3},
  {"FriendInvitationMissionMaster", _k_FriendInvitationMissionMaster, 8},
  {"FriendInvitationMissionRewardMaster", _k_FriendInvitationMissionRewardMaster, 3},
  {"FriendInvitationMissionStageMaster", _k_FriendInvitationMissionStageMaster, 4},
  {"GachaBonusThing", _k_GachaBonusThing, 4},
  {"GachaDetailBonusThing", _k_GachaDetailBonusThing, 4},
  {"GachaDetailMaster", _k_GachaDetailMaster, 12},
  {"GachaMaster", _k_GachaMaster, 24},
  {"GachaRollBonus", _k_GachaRollBonus, 5},
  {"GachaTextTemplateMaster", _k_GachaTextTemplateMaster, 2},
  {"GachaThing", _k_GachaThing, 6},
  {"GameHintMaster", _k_GameHintMaster, 3},
  {"GhostLiveMaster", _k_GhostLiveMaster, 6},
  {"GradualMissionGroupMaster", _k_GradualMissionGroupMaster, 4},
  {"GradualMissionMaster", _k_GradualMissionMaster, 2},
  {"HeadDirectionMaster", _k_HeadDirectionMaster, 2},
  {"HeadMotionMaster", _k_HeadMotionMaster, 2},
  {"HomeBGMDetailMaster", _k_HomeBGMDetailMaster, 7},
  {"HomeBGMMaster", _k_HomeBGMMaster, 7},
  {"HomeBackgroundMaster", _k_HomeBackgroundMaster, 4},
  {"HomeCharacterMaster", _k_HomeCharacterMaster, 4},
  {"HomeCharacterVoicePeriodMaster", _k_HomeCharacterVoicePeriodMaster, 7},
  {"HomePosterMaster", _k_HomePosterMaster, 5},
  {"HomeSkinMaster", _k_HomeSkinMaster, 7},
  {"IconFrameMaster", _k_IconFrameMaster, 6},
  {"JewelShopCategoryMaster", _k_JewelShopCategoryMaster, 6},
  {"JewelShopItemMaster", _k_JewelShopItemMaster, 32},
  {"JewelShopThing", _k_JewelShopThing, 5},
  {"LeaderSenseDetailConditionMaster", _k_LeaderSenseDetailConditionMaster, 5},
  {"LeaderSenseDetailMaster", _k_LeaderSenseDetailMaster, 2},
  {"LeaderSenseMaster", _k_LeaderSenseMaster, 3},
  {"LeagueAllClassGlobalRankingRewardThingMaster", _k_LeagueAllClassGlobalRankingRewardThingMaster, 3},
  {"LeagueClassGroupMaster", _k_LeagueClassGroupMaster, 2},
  {"LeagueClassMaster", _k_LeagueClassMaster, 11},
  {"LeagueGroupRankingRewardPackageMaster", _k_LeagueGroupRankingRewardPackageMaster, 6},
  {"LeagueGroupRankingRewardThingMaster", _k_LeagueGroupRankingRewardThingMaster, 3},
  {"LeagueMaster", _k_LeagueMaster, 10},
  {"LeaguePlayRewardPackageMaster", _k_LeaguePlayRewardPackageMaster, 2},
  {"LeaguePlayRewardThingMaster", _k_LeaguePlayRewardThingMaster, 3},
  {"LeagueRewardPackageMaster", _k_LeagueRewardPackageMaster, 2},
  {"LeagueRewardThingMaster", _k_LeagueRewardThingMaster, 3},
  {"LessonScoreRewardGroupMaster", _k_LessonScoreRewardGroupMaster, 2},
  {"LessonScoreRewardMaster", _k_LessonScoreRewardMaster, 4},
  {"LightLoadSplitEffectMaster", _k_LightLoadSplitEffectMaster, 3},
  {"LipSyncMaster", _k_LipSyncMaster, 2},
  {"LiveDropFrameGroupMaster", _k_LiveDropFrameGroupMaster, 2},
  {"LiveDropFrameMaster", _k_LiveDropFrameMaster, 10},
  {"LiveDropThingMaster", _k_LiveDropThingMaster, 5},
  {"LoginBonusSpineCostumeMaster", _k_LoginBonusSpineCostumeMaster, 5},
  {"LoopMotionMaster", _k_LoopMotionMaster, 5},
  {"MarketFrameThingMaster", _k_MarketFrameThingMaster, 8},
  {"MissionPassLoopRewardMaster", _k_MissionPassLoopRewardMaster, 5},
  {"MissionPassLoopRewardThingMaster", _k_MissionPassLoopRewardThingMaster, 6},
  {"MultiLiveScheduleMaster", _k_MultiLiveScheduleMaster, 8},
  {"MusicCourseDetailMaster", _k_MusicCourseDetailMaster, 7},
  {"MusicCourseMaster", _k_MusicCourseMaster, 13},
  {"MusicCourseRewardGroupMaster", _k_MusicCourseRewardGroupMaster, 4},
  {"MusicCourseRewardThing", _k_MusicCourseRewardThing, 3},
  {"MusicGroupDetailMaster", _k_MusicGroupDetailMaster, 3},
  {"MusicGroupMaster", _k_MusicGroupMaster, 2},
  {"MusicVideoDefaultCostumeGroupMaster", _k_MusicVideoDefaultCostumeGroupMaster, 2},
  {"MusicVideoDefaultCostumeMaster", _k_MusicVideoDefaultCostumeMaster, 3},
  {"MusicVideoMaster", _k_MusicVideoMaster, 11},
  {"MusicVideoOriginalCharacterMaster", _k_MusicVideoOriginalCharacterMaster, 3},
  {"NameBaseColorMaster", _k_NameBaseColorMaster, 6},
  {"NgWordMaster", _k_NgWordMaster, 2},
  {"PermanentMarketThingMaster", _k_PermanentMarketThingMaster, 13},
  {"PhotoEffectChangeItemMaster", _k_PhotoEffectChangeItemMaster, 4},
  {"PhotoEffectMaster", _k_PhotoEffectMaster, 7},
  {"PhotoEffectTypeGroupMaster", _k_PhotoEffectTypeGroupMaster, 4},
  {"PhotoEffectVarietyChangeDetailMaster", _k_PhotoEffectVarietyChangeDetailMaster, 5},
  {"PhotoEffectVarietyUpDetailMaster", _k_PhotoEffectVarietyUpDetailMaster, 4},
  {"PhotoLevelUpItemGroupMaster", _k_PhotoLevelUpItemGroupMaster, 5},
  {"PhotoLevelUpItemMaster", _k_PhotoLevelUpItemMaster, 2},
  {"PhotoSpotMaster", _k_PhotoSpotMaster, 5},
  {"PickupCharacterMissionDetailGroupMaster", _k_PickupCharacterMissionDetailGroupMaster, 2},
  {"PickupCharacterMissionDetailMaster", _k_PickupCharacterMissionDetailMaster, 6},
  {"PickupCharacterMissionDetailRewardMaster", _k_PickupCharacterMissionDetailRewardMaster, 3},
  {"PickupCharacterMissionMaster", _k_PickupCharacterMissionMaster, 6},
  {"PickupSelectionGachaMaster", _k_PickupSelectionGachaMaster, 5},
  {"PlayerRankCapDetailMaster", _k_PlayerRankCapDetailMaster, 2},
  {"PlayerRankCapMaster", _k_PlayerRankCapMaster, 3},
  {"PlayerRankMaster", _k_PlayerRankMaster, 5},
  {"PosterAbilityMaster", _k_PosterAbilityMaster, 13},
  {"ResultVoiceMaster", _k_ResultVoiceMaster, 6},
  {"RouletteEventMaster", _k_RouletteEventMaster, 3},
  {"RouletteMaster", _k_RouletteMaster, 10},
  {"RoulettePrizeMaster", _k_RoulettePrizeMaster, 4},
  {"RoulettePrizeThingMaster", _k_RoulettePrizeThingMaster, 3},
  {"RouletteRollRewardMaster", _k_RouletteRollRewardMaster, 2},
  {"RouletteRollRewardThing", _k_RouletteRollRewardThing, 3},
  {"SceneCameraMaster", _k_SceneCameraMaster, 8},
  {"SenseBranchMaster", _k_SenseBranchMaster, 3},
  {"SenseNotationBuffMaster", _k_SenseNotationBuffMaster, 5},
  {"SenseNotationDetailMaster", _k_SenseNotationDetailMaster, 3},
  {"SenseNotationMaster", _k_SenseNotationMaster, 3},
  {"SensePerformanceCharacterMaster", _k_SensePerformanceCharacterMaster, 4},
  {"SensePerformanceMaster", _k_SensePerformanceMaster, 4},
  {"SignMaster", _k_SignMaster, 3},
  {"SpecialEpisodeMaster", _k_SpecialEpisodeMaster, 4},
  {"SpecialEventLayout", _k_SpecialEventLayout, 18},
  {"SpecialEventMaster", _k_SpecialEventMaster, 6},
  {"SpecialStoryMaster", _k_SpecialStoryMaster, 7},
  {"SplashMaster", _k_SplashMaster, 10},
  {"StaminaRecoveryItemMaster", _k_StaminaRecoveryItemMaster, 4},
  {"StarActBranchMaster", _k_StarActBranchMaster, 3},
  {"StarActConditionMaster", _k_StarActConditionMaster, 6},
  {"StarActMaster", _k_StarActMaster, 12},
  {"StepupGachaGroupMaster", _k_StepupGachaGroupMaster, 2},
  {"StepupGachaMaster", _k_StepupGachaMaster, 2},
  {"StoryEventBonusCharacterBaseMaster", _k_StoryEventBonusCharacterBaseMaster, 4},
  {"StoryEventCircleHighScoreRewardMaster", _k_StoryEventCircleHighScoreRewardMaster, 6},
  {"StoryEventCircleMaster", _k_StoryEventCircleMaster, 3},
  {"StoryEventCircleMissionMaster", _k_StoryEventCircleMissionMaster, 7},
  {"StoryEventCircleMissionRewardMaster", _k_StoryEventCircleMissionRewardMaster, 7},
  {"StoryEventEpisodeMaster", _k_StoryEventEpisodeMaster, 7},
  {"StoryEventHighScoreBuffMaster", _k_StoryEventHighScoreBuffMaster, 6},
  {"StoryEventHighScoreBuffPatternGroupMaster", _k_StoryEventHighScoreBuffPatternGroupMaster, 2},
  {"StoryEventHighScoreBuffPatternMaster", _k_StoryEventHighScoreBuffPatternMaster, 2},
  {"StoryEventHighScoreBuffSettingMaster", _k_StoryEventHighScoreBuffSettingMaster, 4},
  {"StoryEventHighScoreMaster", _k_StoryEventHighScoreMaster, 5},
  {"StoryEventHighScoreRewardMaster", _k_StoryEventHighScoreRewardMaster, 6},
  {"StoryEventMaster", _k_StoryEventMaster, 12},
  {"StoryEventRewardItemPackage", _k_StoryEventRewardItemPackage, 2},
  {"StoryEventRewardMaster", _k_StoryEventRewardMaster, 5},
  {"StoryEventRewardThing", _k_StoryEventRewardThing, 3},
  {"StoryEventStoryBgmGroupMaster", _k_StoryEventStoryBgmGroupMaster, 2},
  {"StoryEventStoryBgmSchedule", _k_StoryEventStoryBgmSchedule, 3},
  {"StoryEventStoryMaster", _k_StoryEventStoryMaster, 6},
  {"StoryEventTotalPointRewardMaster", _k_StoryEventTotalPointRewardMaster, 6},
  {"StoryRelationMaster", _k_StoryRelationMaster, 5},
  {"TeamChallengeMaster", _k_TeamChallengeMaster, 8},
  {"TheaterChapterMaster", _k_TheaterChapterMaster, 9},
  {"TheaterDetailMaster", _k_TheaterDetailMaster, 4},
  {"TheaterRoleMaster", _k_TheaterRoleMaster, 4},
  {"TheaterStoryMaster", _k_TheaterStoryMaster, 9},
  {"TimeLimitedControlMaster", _k_TimeLimitedControlMaster, 2},
  {"TipMaster", _k_TipMaster, 4},
  {"TitleBackgroundDetailMaster", _k_TitleBackgroundDetailMaster, 5},
  {"TitleBackgroundMaster", _k_TitleBackgroundMaster, 5},
  {"TitleCallVoiceMaster", _k_TitleCallVoiceMaster, 5},
  {"TitleDecorationMaster", _k_TitleDecorationMaster, 4},
  {"TotalPointEventMaster", _k_TotalPointEventMaster, 2},
  {"TotalPointEventRewardMaster", _k_TotalPointEventRewardMaster, 7},
  {"TournamentDetailMaster", _k_TournamentDetailMaster, 3},
  {"TournamentMaster", _k_TournamentMaster, 4},
  {"TournamentQualifyingMaster", _k_TournamentQualifyingMaster, 6},
  {"TrialPartyAccessoryMaster", _k_TrialPartyAccessoryMaster, 5},
  {"TrialPartyCharacterMaster", _k_TrialPartyCharacterMaster, 8},
  {"TrialPartyEventMaster", _k_TrialPartyEventMaster, 7},
  {"TrialPartyEventStageMaster", _k_TrialPartyEventStageMaster, 15},
  {"TrialPartyEventStageRewardMaster", _k_TrialPartyEventStageRewardMaster, 3},
  {"TrialPartyMaster", _k_TrialPartyMaster, 3},
  {"TrialPartyPosterMaster", _k_TrialPartyPosterMaster, 5},
  {"TrialPartySlotMaster", _k_TrialPartySlotMaster, 8},
  {"TripleCastAllClassGlobalRankingRewardThingMaster", _k_TripleCastAllClassGlobalRankingRewardThingMaster, 3},
  {"TripleCastGroupRankingRewardPackageMaster", _k_TripleCastGroupRankingRewardPackageMaster, 6},
  {"TripleCastGroupRankingRewardThingMaster", _k_TripleCastGroupRankingRewardThingMaster, 3},
  {"TripleCastMaster", _k_TripleCastMaster, 15},
  {"UnlockConditionMaster", _k_UnlockConditionMaster, 4},
  {"LeagueAllClassGlobalRankingRewardPackageMaster", _k_LeagueAllClassGlobalRankingRewardPackageMaster, 6},
  {"TripleCastAllClassGlobalRankingRewardPackageMaster", _k_TripleCastAllClassGlobalRankingRewardPackageMaster, 6},
};
static const int KEYS_TABLE_COUNT = 723;
}  // namespace wire
