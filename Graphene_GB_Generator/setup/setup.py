
from setuptools import setup, Extension
import numpy

# 定义两个 C 扩展模块
ext_modules = [
    Extension("_cGBUtils", ["_cGBUtils.c"]),
    Extension("_cPolyUtils", ["_cPolyUtils.c"])
]

setup(
    name="GrapheneGenerator",
    ext_modules=ext_modules,
    include_dirs=[numpy.get_include()], # 这行会自动找到 numpy 的头文件位置
)