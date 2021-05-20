from typedefs import coms


def parse_str(in_str: str) -> dict:
    """ Parses a string into a dict containing a datatype and data
    This is heavily inspired from a possible C implementation using structs
    In C, the data should be represented as a long, in order to avoid dynamic allocation
    In the struct will not be returned, but instead passed by reference

    :param in_str: User
    :return: {"commandtype": None, "data": 0}
    """
    ret_dict = {"commandtype": None, "data": 0}
    split_str = in_str.split(" ")
    # This will be done using a switch case in C
    ret_dict["commandtype"] = coms.get_nm(split_str[0])
    ret_dict["data"] = " ".join(split_str[1:])
    return ret_dict
