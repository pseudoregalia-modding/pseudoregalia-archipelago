from .bases import PseudoValidationBase


class TestValidationItemMapping(PseudoValidationBase):
    cases = {
        "non existant item reference": PseudoValidationBase.Case(
            raw_data="""
            item_mapping:
              non_existant_item: pseudo_item
            """,
            expect_errors=True,
        ),
        "non progression item reference": PseudoValidationBase.Case(
            raw_data="""
            item_mapping:
              Good Graces: pseudo_item
            """,
            expect_errors=True,
        ),
    }


class TestValidationTags(PseudoValidationBase):
    cases = {
        "tag with no level descriptions": PseudoValidationBase.Case(
            raw_data="""
            tags:
              - name: tag
                description: tag description
            """,
            expect_errors=True,
        ),
        "tag with advanced and a description": PseudoValidationBase.Case(
            raw_data="""
            tags:
              - name: tag
                description: tag description
                advanced: level description
            """,
            expect_errors=True,
        ),
        "tag with hard and a description": PseudoValidationBase.Case(
            raw_data="""
            tags:
              - name: tag
                description: tag description
                hard: level description
            """,
            expect_errors=True,
        ),
        "tag with expert and a description": PseudoValidationBase.Case(
            raw_data="""
            tags:
              - name: tag
                description: tag description
                expert: level description
            """,
            expect_errors=True,
        ),
        "tag with lunatic and a description": PseudoValidationBase.Case(
            raw_data="""
            tags:
              - name: tag
                description: tag description
                lunatic: level description
            """,
            expect_errors=True,
        ),
        "tag with multiple levels but no description": PseudoValidationBase.Case(
            raw_data="""
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
            raw_data="""
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
            raw_data="""
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
            raw_data="""
            tag_groups:
              - name: tag_group
                description: a tag group
                children: []
            """,
            expect_errors=True,
        ),
        "child that is not a tag": PseudoValidationBase.Case(
            raw_data="""
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
            raw_data="""
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
            raw_data="""
            ref_rules:
              - name: ref_rule
                rule: {}
              - name: ref_rule
                rule: {}
            """,
            expect_errors=True,
        ),
        "ref rule referenced before definition": PseudoValidationBase.Case(
            raw_data="""
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
            raw_data="""
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
        # TODO: CHECK: region names are unique
        # TODO: CHECK: exit region matches the name of a region
        # TODO: CHECK: entrance names are unique
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
        # TODO: CHECK: locations have exactly one of {code, event_item} defined
        # TODO: CHECK: location names are unique
        # TODO: CHECK: location codes are unique
        # TODO: CHECK: location regions match existing regions
        # TODO: CHECK: event items match the name of a progression item
        # TODO: CHECK: event items don't have a code
        # TODO validate checks in location rules?
    }


class TestValidationCompletionRule(PseudoValidationBase):
    cases = {
        # TODO: CHECK: at most one rule type is defined
        # TODO: CHECK: item reference in has matches a progression item or pseudo item
        # TODO: CHECK: can_reach_region value matches the name of a region
        # TODO: CHECK: ref value matches the name of a ref rule
        # TODO: CHECK: tag name matches a tag
        # TODO: CHECK: tag value matches a defined level for that tag
        # TODO: CHECK: option key corresponds to an option in PseudoregaliaOptions
        # TODO: CHECK: option bool values correspond to Toggle options
        # TODO: CHECK: option str values correspond to Choice options
        # TODO: CHECK: option str values match an attribute on the Choice option object
    }
