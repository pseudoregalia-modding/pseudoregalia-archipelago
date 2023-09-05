from typing import Dict, List, NamedTuple

class PseudoregaliaRegionData(NamedTuple):
    connecting_regions: List[str] = []

region_data_table: Dict[str, PseudoregaliaRegionData] = {
    "Menu": PseudoregaliaRegionData(["The Dream"]),
    "The Dream": PseudoregaliaRegionData()
}
