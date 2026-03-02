- for refactoring:
  - add some function documentation to header files
  - make FNames for actor and function names constants and replace them in FindObject calls for performance
    - this turned out to be a pain in the ass because it seems FNames can only be created after unreal_init; will revisit this if performance problems arise

- see if there's a way to enforce a minimum apworld version from the client, so that people can't connect to an 0.6.0 seed with an 0.7.0 client and fuck up location ids
  - could i set a maximum client version in the apworld?

- maybe make the player obtain the game end event for fun

- have some custom death link messages check options or map name
