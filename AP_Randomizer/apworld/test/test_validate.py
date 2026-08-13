from .bases import PseudoValidationBase


class TestValidationItemMapping(PseudoValidationBase):
    cases = {
        "non existant item reference": PseudoValidationBase.Case(
            data="""
            item_mapping:
              non_existant_item: pseudo_item
            """,
            expected_errors=1,
        ),
        "non progression item reference": PseudoValidationBase.Case(
            data="""
            item_mapping:
              Good Graces: pseudo_item
            """,
            expected_errors=1,
        ),
    }


class TestValidationTags(PseudoValidationBase):
    cases = {
        "tag with no level descriptions": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
            """,
            expected_errors=1,
        ),
        "tag with advanced and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                advanced: level description
            """,
            expected_errors=1,
        ),
        "tag with hard and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                hard: level description
            """,
            expected_errors=1,
        ),
        "tag with expert and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                expert: level description
            """,
            expected_errors=1,
        ),
        "tag with lunatic and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                lunatic: level description
            """,
            expected_errors=1,
        ),
        "tag with multiple levels but no description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: level description
                hard: level description
                expert: level description
                lunatic: level description
            """,
            expected_errors=1,
        ),
        "non unique tag names": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: level description
              - name: tag
                advanced: level description
            """,
            expected_errors=1,
        ),
    }


class TestValidationTagGroups(PseudoValidationBase):
    cases = {
        "non unique tag group names": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: tag description
            tag_groups:
              - name: tag_group
                description: a tag group
                children: [tag]
              - name: tag_group
                description: a tag group
                children: [tag]
            """,
            expected_errors=1,
        ),
        "tag group with no children": PseudoValidationBase.Case(
            data="""
            tag_groups:
              - name: tag_group
                description: a tag group
                children: []
            """,
            expected_errors=1,
        ),
        "child that is not a tag": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: tag description
            tag_groups:
              - name: tag_group
                description: a tag group
                children: [not_a_tag]
            """,
            expected_errors=1,
        ),
        "child tag group not previously defined": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: tag description
            tag_groups:
              - name: tag_group_1
                description: a tag group
                children: [tag_group_2]
              - name: tag_group_2
                description: another tag group
                children: [tag]
            """,
            expected_errors=1,
        ),
        "tag group that references itself as child": PseudoValidationBase.Case(
            data="""
            tag_groups:
              - name: tag_group
                description: a tag group
                children: [tag_group]
            """,
            expected_errors=1,
        ),
    }


class TestValidationRefRules(PseudoValidationBase):
    cases = {
        "non unique ref rule names": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: ref_rule
                rule: {}
              - name: ref_rule
                rule: {}
            """,
            expected_errors=1,
        ),
        "ref rule referenced before definition": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: parent_rule
                rule:
                  ref: child_rule
              - name: child_rule
                rule: {}
            """,
            expected_errors=1,
        ),
        "ref rule referenced before definition in list": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: grandparent_rule
                rule: {}
              - name: parent_rule
                rule:
                  ref:
                    - grandparent_rule
                    - child_rule
              - name: child_rule
                rule: {}
            """,
            expected_errors=1,
        ),
        "ref rule references itself": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: ref_rule
                rule:
                  ref: ref_rule
            """,
            expected_errors=1,
        ),
        # TODO: add test cases to deeply validate rule checks in ref_rules?
    }


class TestValidationRegions(PseudoValidationBase):
    cases = {
        "non unique region names": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
              - name: Region
            """,
            expected_errors=1,
        ),
        "non existant exit region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region A
              - name: Region B
                exits:
                  - region: Region C
            """,
            expected_errors=1,
        ),
        "non unique entrance name with default name": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region A
              - name: Region B
                exits:
                  - region: Region A
                  - region: Region A
            """,
            expected_errors=1,
        ),
        "non unique entrance name with explicit name": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region A
              - name: Region B
              - name: Region C
                exits:
                  - region: Region A
                    entrance_name: Entrance
                  - region: Region B
                    entrance_name: Entrance
            """,
            expected_errors=1,
        ),
        # TODO: validate checks in entrance rules?
    }


class TestValidationEnums(PseudoValidationBase):
    cases = {
        "repeated string in player start enum": PseudoValidationBase.Case(
            data="""
            enums:
              player_start:
                - PlayerStart
                - PlayerStart
            """,
            expected_errors=1,
        ),
    }


class TestValidationSpawnPoints(PseudoValidationBase):
    cases = {
        "more than one spawn point marked as default": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region1
              - name: Region2
            enums:
              player_start:
                - PlayerStart1
                - PlayerStart2
            spawn_points:
              - name: spawn_1
                player_start: PlayerStart1
                region: Region1
                default: true
              - name: spawn_2
                player_start: PlayerStart2
                region: Region2
                default: true
            """,
            expected_errors=1,
        ),
        "non unique spawn point names": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region1
              - name: Region2
            enums:
              player_start:
                - PlayerStart1
                - PlayerStart2
            spawn_points:
              - name: spawn_1
                player_start: PlayerStart1
                region: Region1
                default: true
              - name: spawn_1
                player_start: PlayerStart2
                region: Region2
            """,
            expected_errors=1,
        ),
        "spawn point with invalid player start": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            enums:
              player_start:
                - PlayerStart1
            spawn_points:
              - name: spawn
                player_start: FakePlayerStart
                region: Region
                default: true
            """,
            expected_errors=1,
        ),
        "spawn point with invalid region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            enums:
              player_start:
                - PlayerStart1
            spawn_points:
              - name: spawn
                player_start: PlayerStart1
                region: Fake Region
                default: true
            """,
            expected_errors=1,
        ),
    }


class TestValidationLocations(PseudoValidationBase):
    cases = {
        "location with no code or event item": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                region: Region
            """,
            expected_errors=1,
        ),
        "location with both code and event item": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                code: 1
                region: Region
                event_item: Something Worth Being Awake For
            """,
            expected_errors=1,
        ),
        "non unique location names": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                code: 1
                region: Region
              - name: Location
                code: 2
                region: Region
            """,
            expected_errors=1,
        ),
        "non unique location codes": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location 1
                code: 1
                region: Region
              - name: Location 2
                code: 1
                region: Region
            """,
            expected_errors=1,
        ),
        "location references non existant region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                code: 1
                region: Fake Region
            """,
            expected_errors=1,
        ),
        "non progression event item": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                region: Region
                event_item: Good Graces
            """,
            expected_errors=1,
        ),
        "progression event item with code": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            locations:
              - name: Location
                region: Region
                event_item: Slide
            """,
            expected_errors=1,
        ),
        # TODO: validate checks in location rules?
        # TODO: validate can_create options?
    }


class TestValidationCompletionRule(PseudoValidationBase):
    cases = {
        "error within and": PseudoValidationBase.Case(
            data="""
            completion_rule:
              and:
                - ref: not_real
            """,
            expected_errors=1,
        ),
        "error within or": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - ref: not_real
            """,
            expected_errors=1,
        ),
        "has references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: Fake Item
            """,
            expected_errors=1,
        ),
        "has references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: Good Graces
            """,
            expected_errors=1,
        ),
        "has all references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: [Fake Item, Slide]
            """,
            expected_errors=1,
        ),
        "has all references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: [Good Graces, Slide]
            """,
            expected_errors=1,
        ),
        "has all counts references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has:
                Fake Item: 2
            """,
            expected_errors=1,
        ),
        "has all counts references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has:
                Good Graces: 2
            """,
            expected_errors=1,
        ),
        "can reach region references unknown region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            completion_rule:
              can_reach_region: Fake Region
            """,
            expected_errors=1,
        ),
        "unknown ref rule": PseudoValidationBase.Case(
            data="""
            completion_rule:
              ref: unknown_ref
            """,
            expected_errors=1,
        ),
        "unknown ref rule in list": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: real_ref
                rule: {}
            completion_rule:
              ref: [real_ref, unknown_ref]
            """,
            expected_errors=1,
        ),
        "tags references unknown tag": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: tag description
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  tags:
                    fake_tag: advanced
            """,
            expected_errors=1,
        ),
        "tag level is undefined (has no description)": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: tag description
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  tags:
                    tag: hard
            """,
            expected_errors=1,
        ),
        "option referenced does not exist": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  options:
                    fake_option: true
            """,
            expected_errors=1,
        ),
        "option with bool value but isn't a Toggle": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  options:
                    game_version: true
            """,
            expected_errors=1,
        ),
        "option with str value but isn't a Choice": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  options:
                    obscure_logic: yeah
            """,
            expected_errors=1,
        ),
        "option with str value but isn't an option in the Choice": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - has: [Slide, Sunsetter]
                - has: Slide
                  options:
                    spawn_point: fake_spawn
            """,
            expected_errors=1,
        ),
    }
