from dataclasses import dataclass, field

import yaml
from Fill import fast_fill

from test.bases import WorldTestBase

from .. import PseudoregaliaWorld
from ..logic import PseudoregaliaData, data_from_dict, pseudoregalia_data
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
                   f"Expected {expected_key_hints} but found {slot_data['key_hints']}"
        else:
            assert "key_hints" not in slot_data, "Expected no key_hints in slot_data"


class PseudoValidationBase(PseudoTestBase):
    run_default_tests = False

    @dataclass
    class Case:
        data: PseudoregaliaData | str
        expected_errors: int = field(default=0)

    cases: dict[str, Case] = {
        "real data": Case(data=pseudoregalia_data),
    }

    def test_validate(self):
        for case_name, case_data in self.cases.items():
            with self.subTest(case_name, case_data=case_data):
                if isinstance(case_data.data, PseudoregaliaData):
                    data = case_data.data
                elif isinstance(case_data.data, str):
                    yaml_dict = yaml.safe_load(case_data.data)
                    raw = {"item_mapping": {}, "tags": [], "tag_groups": [], "ref_rules": [], "regions": [],
                           "enums": {"player_start": []}, "spawn_points": [], "locations": [], "completion_rule": {}}
                    raw.update(yaml_dict)
                    data = data_from_dict(raw)

                validator = Validator()
                validator.validate_data(data)
                num_errors = len(validator.errors)
                nl = "\n"  # py<3.12 escape characters didn't work in fstrings ig
                assert case_data.expected_errors == num_errors, \
                    f"expected {case_data.expected_errors} errors, got {num_errors}{nl}{nl.join(validator.errors)}"
