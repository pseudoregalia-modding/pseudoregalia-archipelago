from .rules import PseudoregaliaRulesHelpers


class PseudoregaliaNormalRules(PseudoregaliaRulesHelpers):
    def __init__(self, world) -> None:
        super().__init__(world)

        self.region_rules.update({
            "Dungeon Mirror -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Mirror": lambda state:
                self.can_attack(state),
            "Dungeon Slide -> Dungeon Strong Eyes": lambda state:
                self.has_slide(state),
            "Dungeon Slide -> Dungeon Escape Lower": lambda state:
                self.can_attack(state) and self.navigate_darkrooms(state),
            "Dungeon Strong Eyes -> Dungeon Slide": lambda state:
                self.has_slide(state),
            "Dungeon Strong Eyes -> Dungeon => Castle": lambda state:
                self.has_small_keys(state),
            # "Dungeon => Castle -> Dungeon Mirror": lambda state: True,
            "Dungeon => Castle -> Dungeon Strong Eyes": lambda state:
                self.has_small_keys(state),
            # "Dungeon => Castle -> Castle Main": lambda state: True,
            "Dungeon Escape Lower -> Dungeon Slide": lambda state:
                self.can_attack(state),
            "Dungeon Escape Lower -> Dungeon Escape Upper": lambda state:
                self.can_bounce(state)
                or self.get_kicks(state, 1) and self.has_plunge(state)
                or self.get_kicks(state, 3),
            # "Dungeon Escape Lower -> Underbelly => Dungeon": lambda state: True,
            "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            # "Dungeon Escape Upper -> Theatre Outside Scythe Corridor": lambda state: True,
            # "Castle Main -> Dungeon => Castle": lambda state: True,
            # "Castle Main -> Keep Main": lambda state: True,
            # "Castle Main -> Empty Bailey": lambda state: True,
            "Castle Main -> Library Main": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.can_attack(state),
            "Castle Main -> Theatre Pillar": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle Main -> Castle Spiral Climb": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state),
            # "Castle Spiral Climb -> Castle Main": lambda state: True,
            "Castle Spiral Climb -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Castle Spiral Climb -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state),
            "Castle By Scythe Corridor -> Castle Spiral Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4) and self.has_plunge(state),
            "Castle By Scythe Corridor -> Castle High Climb": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state)
                or self.get_kicks(state, 1) and self.has_plunge(state) and self.can_slidejump(state),
            "Castle By Scythe Corridor -> Castle => Theatre (Front)": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 2),
            "Castle => Theatre (Front) -> Castle By Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.get_kicks(state, 4),
            "Castle => Theatre (Front) -> Castle Moon Room": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            # "Castle => Theatre (Front) -> Theatre Main": lambda state: True,
            "Library Main -> Library Locked": lambda state:
                self.has_small_keys(state),
            "Library Main -> Library Greaves": lambda state:
                self.has_slide(state),
            "Library Main -> Library Top": lambda state:
                self.kick_or_plunge(state, 4)
                or self.knows_obscure(state) and self.get_kicks(state, 1) and self.has_plunge(state),
            "Library Greaves -> Library Top": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 2),
            "Library Top -> Library Greaves": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3) and self.has_plunge(state)
                or self.get_kicks(state, 3) and self.can_bounce(state),
            "Keep Main -> Keep Locked Room": lambda state:
                # Note for trackers: This is accessible with nothing but not in logic.
                # Cutting the platform or hitting the lever make this harder and are irreversible.
                # On Hard and above, the player is expected to not do either.
                self.has_small_keys(state)
                or self.get_kicks(state, 3)
                or self.has_plunge(state) and self.get_kicks(state, 1)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_gem(state) and self.get_kicks(state, 1),
            "Keep Main -> Keep Sunsetter": lambda state:
                # See "Keep Main -> Keep Locked Room".
                # All other methods would go through Keep Locked Room instead.
                self.has_gem(state),
            "Keep Main -> Keep => Underbelly": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            "Keep Main -> Theatre Outside Scythe Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_bounce(state)
                or self.can_slidejump(state),
            "Keep Main -> Keep Path To Throne": lambda state:
                self.has_breaker(state),
            # "Keep Locked Room -> Keep Sunsetter": lambda state: True,
            # "Keep => Underbelly -> Keep Main": lambda state: True,
            # "Keep => Underbelly -> Underbelly Hole": lambda state: True,
            # "Underbelly => Dungeon -> Dungeon Escape Lower": lambda state: True,
            # "Underbelly => Dungeon -> Underbelly Light Pillar": lambda state: True,
            "Underbelly => Dungeon -> Underbelly Ascendant Light": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_breaker(state),
            # "Underbelly Light Pillar -> Underbelly Main Upper": lambda state: True,
            "Underbelly Light Pillar -> Underbelly => Dungeon": lambda state:
                self.can_bounce(state)
                or self.kick_or_plunge(state, 4),
            "Underbelly Light Pillar -> Underbelly Ascendant Light": lambda state:
                self.has_breaker(state)
                and (
                    self.has_plunge(state)
                    or self.get_kicks(state, 4))
                or self.knows_obscure(state) and self.has_gem(state) and self.get_kicks(state, 1),
            # "Underbelly Ascendant Light -> Underbelly Light Pillar": lambda state: True,
            "Underbelly Ascendant Light -> Underbelly => Dungeon": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state),
            # "Underbelly Main Lower -> Underbelly Little Guy": lambda state: True,
            "Underbelly Main Lower -> Underbelly Hole": lambda state:
                self.has_plunge(state)
                and (
                    self.get_kicks(state, 1)
                    or self.can_slidejump(state)
                    or self.can_attack(state)),
            "Underbelly Main Lower -> Underbelly By Heliacal": lambda state:
                self.has_slide(state) and self.has_plunge(state),
            "Underbelly Main Lower -> Underbelly Main Upper": lambda state:
                self.has_plunge(state)
                and (
                    self.get_kicks(state, 2)
                    or self.get_kicks(state, 1) and self.has_gem(state)),
            # "Underbelly Main Upper -> Underbelly Main Lower": lambda state: True,
            "Underbelly Main Upper -> Underbelly Light Pillar": lambda state:
                self.has_breaker(state) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 2)
                or self.has_gem(state)
                and (
                    self.get_kicks(state, 2) and self.has_plunge(state)
                    or self.get_kicks(state, 4)),
            "Underbelly Main Upper -> Underbelly By Heliacal": lambda state:
                self.has_breaker(state)
                and (
                    state.has("Ascendant Light", self.player)
                    or self.can_slidejump(state) and self.get_kicks(state, 3)
                    or self.has_gem(state) and self.get_kicks(state, 2)),
            "Underbelly By Heliacal -> Underbelly Main Upper": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.get_kicks(state, 1)
                    or self.has_gem(state) and self.can_slidejump(state)),
            # "Underbelly Little Guy -> Empty Bailey": lambda state: True,
            "Underbelly Little Guy -> Underbelly Main Lower": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 1),
            # "Underbelly => Keep -> Keep => Underbelly": lambda state: True,
            "Underbelly => Keep -> Underbelly Hole": lambda state:
                self.has_plunge(state),
            "Underbelly Hole -> Underbelly Main Lower": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.can_slidejump(state)
                or self.can_attack(state),
            "Underbelly Hole -> Underbelly => Keep": lambda state:
                self.has_slide(state),
        })

        self.location_rules.update({
            # "Dilapidated Dungeon - Dream Breaker": lambda state: True,
            # "Dilapidated Dungeon - Slide": lambda state: True,
            # "Dilapidated Dungeon - Alcove Near Mirror": lambda state: True,
            "Dilapidated Dungeon - Dark Orbs": lambda state:
                self.has_gem(state) and self.can_bounce(state)
                or self.has_gem(state) and self.kick_or_plunge(state, 3)
                or self.get_kicks(state, 2) and self.can_bounce(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1) and self.can_bounce(state),
            "Dilapidated Dungeon - Past Poles": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 3),
            "Dilapidated Dungeon - Rafters": lambda state:
                self.kick_or_plunge(state, 3)
                or self.knows_obscure(state) and self.can_bounce(state) and self.has_gem(state),
            "Dilapidated Dungeon - Strong Eyes": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state)
                and (
                    self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                    or self.has_gem(state) and self.get_kicks(state, 3)),
            # "Castle Sansa - Indignation": lambda state: True,
            "Castle Sansa - Alcove Near Dungeon": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.kick_or_plunge(state, 2),
            "Castle Sansa - Balcony": lambda state:
                self.has_gem(state)
                or self.kick_or_plunge(state, 3)
                or self.can_slidejump(state) and self.kick_or_plunge(state, 2),
            "Castle Sansa - Corner Corridor": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 4),
            "Castle Sansa - Floater In Courtyard": lambda state:
                self.can_bounce(state) and self.has_plunge(state)
                or self.can_bounce(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.get_kicks(state, 4)
                or self.knows_obscure(state) and self.can_bounce(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_gem(state) and self.get_kicks(state, 1),
            "Castle Sansa - Locked Door": lambda state:
                self.has_small_keys(state),
            "Castle Sansa - Platform In Main Halls": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2),
            "Castle Sansa - Tall Room Near Wheel Crawlers": lambda state:
                self.has_gem(state) and self.kick_or_plunge(state, 1)
                or self.get_kicks(state, 2),
            "Castle Sansa - Wheel Crawlers": lambda state:
                self.can_bounce(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.get_kicks(state, 1) and self.can_slidejump(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Castle Sansa - High Climb From Courtyard": lambda state:
                self.get_kicks(state, 2)
                or self.has_gem(state) and self.has_plunge(state)
                or self.has_breaker(state) and self.get_kicks(state, 1)
                or self.knows_obscure(state) and self.has_plunge(state) and self.get_kicks(state, 1),
            "Castle Sansa - Alcove Near Scythe Corridor": lambda state:
                self.has_gem(state) and self.get_kicks(state, 1) and self.has_plunge(state)
                or self.kick_or_plunge(state, 4),
            "Castle Sansa - Near Theatre Front": lambda state:
                self.get_kicks(state, 4)
                or self.get_kicks(state, 2) and self.has_plunge(state),
            "Listless Library - Sun Greaves": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 1": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 2": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Sun Greaves 3": lambda state:
                self.has_breaker(state)
                or self.knows_obscure(state) and self.has_plunge(state),
            "Listless Library - Upper Back": lambda state:
                (self.has_breaker(state) or self.knows_obscure(state) and self.has_plunge(state))
                and (
                    self.has_gem(state) and self.kick_or_plunge(state, 1)
                    or self.kick_or_plunge(state, 2)),
            "Listless Library - Locked Door Across": lambda state:
                self.has_gem(state)
                or self.get_kicks(state, 1)
                or self.can_slidejump(state),
            "Listless Library - Locked Door Left": lambda state:
                self.has_gem(state)
                or self.can_slidejump(state) and self.get_kicks(state, 1)
                or self.kick_or_plunge(state, 3),
            "Sansa Keep - Near Theatre": lambda state:
                self.kick_or_plunge(state, 1)
                or self.has_gem(state),
            # "Sansa Keep - Alcove Near Locked Door": lambda state: True,
            "Sansa Keep - Levers Room": lambda state:
                self.can_attack(state),
            "Sansa Keep - Sunsetter": lambda state:
                self.can_attack(state),
            "Sansa Keep - Strikebreak": lambda state:
                (self.can_attack(state) and (self.has_slide(state) or self.can_strikebreak(state)))
                and (
                    self.has_gem(state)
                    or self.has_plunge(state) and self.get_kicks(state, 1)
                    or self.get_kicks(state, 3)),
            "Sansa Keep - Lonely Throne": lambda state:
                (self.has_gem(state)
                 and (
                     self.has_plunge(state) and self.get_kicks(state, 1)
                     or self.has_plunge(state) and state.has("Ascendant Light", self.player)
                     or self.get_kicks(state, 1) and state.has("Ascendant Light", self.player))
                 )
                or (state.has("Ascendant Light", self.player) and self.kick_or_plunge(state, 4)),
            # "The Underbelly - Ascendant Light": lambda state: True,
            "The Underbelly - Rafters Near Keep": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 2)
                or self.can_bounce(state),
            "The Underbelly - Locked Door": lambda state:
                self.has_small_keys(state),
            "The Underbelly - Main Room": lambda state:
                self.has_plunge(state)
                or self.has_gem(state)
                or self.get_kicks(state, 2)
                or self.can_slidejump(state) and self.get_kicks(state, 1),
            "The Underbelly - Alcove Near Light": lambda state:
                self.can_attack(state)
                or self.has_gem(state)
                or self.get_kicks(state, 4)
                or self.get_kicks(state, 3) and self.can_slidejump(state),
            "The Underbelly - Building Near Little Guy": lambda state:
                self.has_plunge(state)
                or self.get_kicks(state, 3),
            "The Underbelly - Strikebreak Wall": lambda state:
                self.can_strikebreak(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 4)
                    or self.get_kicks(state, 2) and self.has_plunge(state)),
            "The Underbelly - Surrounded By Holes": lambda state:
                self.can_soulcutter(state)
                and (
                    self.can_bounce(state)
                    or self.get_kicks(state, 2)
                )
                or self.can_slidejump(state) and self.has_gem(state) and self.get_kicks(state, 1),
        })

    def set_pseudoregalia_rules(self) -> None:
        super().set_pseudoregalia_rules()
