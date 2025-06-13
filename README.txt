


TRIVIA::
 - the class `Effect`
   - contrary to its name it does not describe a visual effect / animation; it describes the properties of a skill



TO DO:::

 - DungeonServer: Effect.cpp l:365 + Client:: DungeonProcess.cpp l:948
   - seems like different formulas are used for the overdrive mana charge, should look into it

- DungeonProcess.cpp :: l: 1037 
 - seems like the client computes the weight percentage & does a check on the limit; 
   - this info should come from the server, to be investigated;


- Client.EffectProcess.cpp :: 1574, seems like the client updates the HP based on the poison effect; 
    - should be done on the server


- Client.Effect.cpp l: 455 + DungeonProcess l: 939 
    - seems like the client is handling the cast speed logic ? 


- User. m_dwExp :: represents the total accumulated EXP; (computing the correct level & scale of the xp bar should now be complete)

- Skills + Cooldown points + time [To be investigated more]
  - player cool points seem to represent an amount in seconds, of "available cast time"

  + each skill has a "cool time" in milliseconds : Effect->BaseSkill->dwCoolTime ; 
    - this represents the animation duration;
      - generally there's a quick basic animation for most skills, but fighter & ranger have complex animation ; sorc and summoner have basic animations


    - the player should wait "dwCoolTime / 1000" seconds before casting the same skill again; i.e. wait for the current animation to finish
    - the last cast date( time in milliseconds) for each skill is stored in g_pMainPlayer->d_dwStartSkillTick and it used in checks



- CTDS_DUNGEON_CASTING vs CTDS_SKILL 

  - CTDS_SKILL is sent by the client only if it's overdrive or (fighter) aura recharge; i.e. a continous skill
  - Processed by EffectLayer 
    - on each CTDS_SKILL event { pSkillDesc } ::
      - Process and find out the caster : GetCaster(pSkillDesc) 
        i.e. : 
          -  find out if the caster is a user or a monster or a user's monster
          - special handling when the caster is a user::
             - cond 1: check if the user is not dead (and if so he can only cast __TREEAGION__ (??)) and he is not in a village (IsUnitStatusReadySkill)
             - cond 2: about to do with skill level, basic integrity checks (skill level > 0 , < 255)
             - cond 3: something about user being in god mode, no idea what is that;
             - cond 4: if overdrive then check if there is an item in users inventory that allows it( free pass, taurus zodiac)
             - step: get the mana 'offset', to be added to the user's current mana points for using this skill;
                - mostly this value is negative except when the used skill is aura recharge; that skill actually gives mana
               - special computations:
                  - life explosion uses all of the mana, offset = -user.currentSP()
                  - overdrive has a complicated formula + it also updates the skill 'keepTime' based on the time passed since the skill cast (cast started at user->m_dwTemp[USER_TEMP_CASTINGSTARTTICK] (milliseconds)); let duration = time_now_milliseconds - user->m_dwTemp[USER_TEMP_CASTINGSTARTTICK];
                   - formula:: 
                        spOffset = user.maxSP() * 0.1 * (duration / 1000)
                        skillKeepTime = spOffset / skill.currentManaCost(based on its level, etc) * (weird ego formula) * 1000 (last *1000 probably to turn it into milliseconds)

           - returned offset either positive(if aura recharge) or negative
      - record the time for this current skill event, update the mana
        ```
        	pOwnUser->m_dwStartSkillTick[pSkillDesc->bSkillKind] = g_dwTickCount;
	        pOwnUser->SetSP(pOwnUser->GetSP()+nMana);
        ```
      - then send a USER_STATUS update to the client; 
        - this means that the client should not deal with computing the mana value at each cast 'step', 
          this will be returned back by the server in a USER_STATUS packet;
        - the client is responsible for sending CTDS_SKILL packets as long as the user keeps the mouse button pressed

  CTDS_DUNGEON_CASTING
    - sent by the client when 
       - left or mouse button press event (DungeonProcess.SkillEvent(0 (left) || 1 (right)))
    + Processing done by the DungeonServer::
        + cond1: if the time since the last cast (g_dwTickCount - pUser->m_dwStartSkillTick[bSkillKind]) is less than the effect's cooldown time (pEffect->dwCoolTime)
          - check if you can 'substract' the effect's cooldown time(milliseconds) from the user's cooldown points(seconds)
            - int nCool = INT(pUser->m_fCurCoolPoint*1000 - pEffect->dwCoolTime); 
            - nCool < 0 ? fail the current skill cast, send packet about it; 
              else
                - pUser->m_fCurCoolPoint = max(nCool/1000.f, 0.1F) and continue;
              (===>the client shouldn't perform cooldown cost calculations, this value should be sent back by the dungeon server)
