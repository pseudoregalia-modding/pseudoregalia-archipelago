from .bases import PseudoValidationBase


class TestValidationItemMapping(PseudoValidationBase):
    cases = {
        "non existant item reference": PseudoValidationBase.Case(
            data="""
            item_mapping:
              non_existant_item: pseudo_item
            """,
            expect_errors=True,
        ),
        "non progression item reference": PseudoValidationBase.Case(
            data="""
            item_mapping:
              Good Graces: pseudo_item
            """,
            expect_errors=True,
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
            expect_errors=True,
        ),
        "tag with advanced and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                advanced: level description
            """,
            expect_errors=True,
        ),
        "tag with hard and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                hard: level description
            """,
            expect_errors=True,
        ),
        "tag with expert and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                expert: level description
            """,
            expect_errors=True,
        ),
        "tag with lunatic and a description": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                description: tag description
                lunatic: level description
            """,
            expect_errors=True,
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
            expect_errors=True,
        ),
        "non unique tag names": PseudoValidationBase.Case(
            data="""
            tags:
              - name: tag
                advanced: level description
              - name: tag
                advanced: level description
            """,
            expect_errors=True,
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
            expect_errors=True,
        ),
        "tag group with no children": PseudoValidationBase.Case(
            data="""
            tag_groups:
              - name: tag_group
                description: a tag group
                children: []
            """,
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
        ),
        "ref rule references itself": PseudoValidationBase.Case(
            data="""
            ref_rules:
              - name: ref_rule
                rule:
                  ref: ref_rule
            """,
            expect_errors=True,
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
            expect_errors=True,
        ),
        "non existant exit region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region A
              - name: Region B
                exits:
                  - region: Region C
            """,
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
        ),
        # TODO: validate checks in entrance rules?
    }


class TestValidationOrigins(PseudoValidationBase):
    cases = {
        # TODO: CHECK: origin spawn points are unique
        # TODO: CHECK: origin names match an attribute on the SpawnPoint object
        # TODO: CHECK: origin regions match existing regions
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
        ),        
        # TODO: validate checks in location rules?
        # TODO: validate can_create options?
    }


class TestValidationCompletionRule(PseudoValidationBase):
    cases = {
        "has and and both defined": PseudoValidationBase.Case(
            data="""
            completion_rule:
              and:
                - has: Slide
                - has: Solar Wind
              has: Sunsetter
            """,
            expect_errors=True,
        ),
        "error with an and": PseudoValidationBase.Case(
            data="""
            completion_rule:
              and:
                - ref: not_real
            """,
            expect_errors=True,
        ),
        "error with an or": PseudoValidationBase.Case(
            data="""
            completion_rule:
              or:
                - ref: not_real
            """,
            expect_errors=True,
        ),
        "has references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: Fake Item
            """,
            expect_errors=True,
        ),
        "has references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: Good Graces
            """,
            expect_errors=True,
        ),
        "has all references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: [Fake Item, Slide]
            """,
            expect_errors=True,
        ),
        "has all references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has: [Good Graces, Slide]
            """,
            expect_errors=True,
        ),
        "has all counts references non item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has:
                Fake Item: 2
            """,
            expect_errors=True,
        ),
        "has all counts references non progression item": PseudoValidationBase.Case(
            data="""
            completion_rule:
              has:
                Good Graces: 2
            """,
            expect_errors=True,
        ),
        "can reach region references unknown region": PseudoValidationBase.Case(
            data="""
            regions:
              - name: Region
            completion_rule:
              can_reach_region: Fake Region
            """,
            expect_errors=True,
        ),
        "unknown ref rule": PseudoValidationBase.Case(
            data="""
            completion_rule:
              ref: unknown_ref
            """,
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
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
            expect_errors=True,
        ),
    }
