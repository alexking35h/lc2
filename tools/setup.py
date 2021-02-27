import setuptools

setuptools.setup(
    name="lc2-tools",
    version=0.01,
    author="Alex King",
    description="Build and test tools for lc2 C compiler",
    packages=setuptools.find_packages(),
    python_requires=">=3.8",
    entry_points={
        "console_scripts": [
            "gen-parser-h=lc2_parser.parser_hdr:main",
            "gen-parser-cpp=lc2_parser.parser_impl:main"
        ]
    }
)