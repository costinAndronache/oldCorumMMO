


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