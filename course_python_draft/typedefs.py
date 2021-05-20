from enum import IntEnum

COMMAND_TYPE = "commandtype"
DAT_PRSD = "data"

class coms(IntEnum):
    SET_WAKE = 1
    SET_SLEEP = 2
    OPEN = 4
    CLOSE = 8
    DE_ACTIVATE = 16
    ERROR = 32
    NOTHING = 64

    @classmethod
    def get_nm(cls, name: str):
        for enum_entry, value in COM_NAMES.items():
            if value == name:
                return enum_entry
        if name == "":
            return cls.NOTHING
        return cls.ERROR

COM_NAMES = {coms.SET_WAKE: "SET_WAKE",
             coms.SET_SLEEP: "SET_SLEEP",
             coms.OPEN: "OPEN",
             coms.CLOSE: "CLOSE",
             coms.DE_ACTIVATE: "DE_ACTIVATE",
             coms.ERROR: "ERROR",
             coms.NOTHING: "NOTHING"}