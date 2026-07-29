from dataclasses import dataclass, field
from test.bases import WorldTestBase
import yaml

from Fill import fast_fill

from .. import PseudoregaliaWorld, pseudoregalia_data
from ..logic import PseudoregaliaData, data_from_dict
from ..validate import Validator


class PseudoTestBase(WorldTestBase):
    game = "Pseudoregalia"
    world: PseudoregaliaWorld


class PseudoKeyHintsBase(PseudoTestBase):
    run_default_tests = False
    expect_hints: bool = True
    major_key_names = [
        "Major Key - Empty Bailey",
        "Major Key - The Underbelly",
        "Major Key - Tower Remains",
        "Major Key - Sansa Keep",
        "Major Key - Twilight Theatre",
    ]

    def test_key_hints(self):
        major_keys = sorted(self.get_items_by_name(self.major_key_names),
                            key=lambda item: self.major_key_names.index(item.name))
        locations = self.multiworld.get_unfilled_locations(self.player)[:5]
        fast_fill(self.multiworld, major_keys, locations)
        slot_data = self.world.fill_slot_data()
        if self.expect_hints:
            assert "key_hints" in slot_data, "Expected key_hints in slot_data"
            expected_key_hints = [
                [{
                    "player": self.player,
                    "location": location.address,
                }]
                for location in locations
            ]
            assert slot_data["key_hints"] == expected_key_hints, \
                   f"Expected {expected_key_hints} but found {slot_data["key_hints"]}"
        else:
            assert "key_hints" not in slot_data, "Expected no key_hints in slot_data"


class PseudoValidationBase(PseudoTestBase):
    run_default_tests = False
    
    @dataclass
    class Case:
        data: PseudoregaliaData | None = field(default=None)
        raw_data: str | None = field(default=None)
        expect_errors: bool = field(default=False)
    
    cases: dict[str, Case] = {
        "real data": Case(data=pseudoregalia_data),
    }

    def test_validate(self):
        for case_name, case_data in self.cases.items():
            with self.subTest(case_name, case_data=case_data):
                assert case_data.data is not None or case_data.raw_data is not None, "no data defined for subtest"

                if case_data.data is not None:
                    data = case_data.data
                else:
                    yaml_dict = yaml.safe_load(case_data.raw_data)
                    raw = {"item_mapping": {}, "tags": [], "tag_groups": [], "ref_rules": [], "regions": [],
                           "origins": [], "locations": [], "completion_rule": {}}
                    raw.update(yaml_dict)
                    data = data_from_dict(raw)

                validator = Validator()
                validator.validate_data(data)
                num_errors = len(validator.errors)
                if case_data.expect_errors:
                    assert num_errors != 0, "expected errors, got none"
                else:
                    assert num_errors == 0, f"expected no errors, got {num_errors}\n{"\n".join(validator.errors)})"
