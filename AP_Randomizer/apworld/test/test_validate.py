from .bases import PseudoValidationBase
from ..logic import PseudoregaliaData, RuleData


class TestValidationItemMappingFailure(PseudoValidationBase):
    # TODO: add a way to create mock data more cleanly
    data = PseudoregaliaData(
        item_mapping={"non_existant_item": "fake_pseudo_item"},
        tags=[],
        tag_groups=[],
        ref_rules=[],
        regions=[],
        origins=[],
        locations=[],
        completion_rule=RuleData(None, None, None, None, None, None, None),
    )
    expect_errors = True

# TODO: add tests for each CHECK in validate.py
