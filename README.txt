


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

- Skills + Cooldown points + time
  - player cool points seem to represent an amount in seconds, of "available cast time"

  + each skill has a "cool time" in milliseconds : Effect->BaseSkill->dwCoolTime ; 
    - this represents the animation duration;
      - generally there's a quick basic animation for most skills, but fighter & ranger have complex animation ; sorc and summoner have basic animations


    - the player should wait "dwCoolTime / 1000" seconds before casting the same skill again; i.e. wait for the current animation to finish
    - the last cast date( time in milliseconds) for each skill is stored in g_pMainPlayer->d_dwStartSkillTick and it used in checks

  - next the skill's 
