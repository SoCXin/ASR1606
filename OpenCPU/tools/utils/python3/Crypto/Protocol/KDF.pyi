from types import ModuleType
from typing import Optional, Callable, Tuple, Union, Dict, Any

RNG = Callable[[int], bytes]

def PBKDF1(password: str, salt: bytes, dkLen: int, count: Optional[int]=1000, hashAlgo: Optional[ModuleType]=None) -> bytes: ...
def PBKDF2(password: str, salt: bytes, dkLen: Optional[int]=16, count: Optional[int]=1000, prf: Optional[RNG]=None, hmac_hash_module: Optional[ModuleType]=None) -> bytes: ...

class _S2V(object):
    def __init__(self, key: bytes, ciphermod: ModuleType, cipher_params: Optional[Dict[Any, Any]]=None) -> None: ...

    @staticmethod
    def new(key: bytes, ciphermod: ModuleType) -> None: ...
    def update(self, item: bytes) -> None: ...
    def derive(self) -> bytes: ...

def HKDF(master: bytes, key_len: int, salt: bytes, hashmod: ModuleType, num_keys: Optional[int]=1, context: Optional[bytes]=None) -> Union[bytes, Tuple[bytes, ...]]: ...

def scrypt(password: str, salt: str, key_len: int, N: int, r: int, p: int, num_keys: Optional[int]=1) -> Union[bytes, Tuple[bytes, ...]]: ...

def _bcrypt_decode(data: bytes) -> bytes: ...
def _bcrypt_hash(password:bytes , cost: int, salt: bytes, constant:bytes, invert:bool) -> bytes: ...
def bcrypt(password: Union[bytes, str], cost: int, salt: Optional[bytes]=None) -> bytes: ...
def bcrypt_check(password: Union[bytes, str], bcrypt_hash: Union[bytes, bytearray, str]) -> None: ...
