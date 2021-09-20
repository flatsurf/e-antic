# Install e-antic in Conda

You can install this package with conda. Download and install
[Miniconda](https://conda.io/miniconda.html), then run

    conda config --add channels conda-forge
    conda config --set channel_priority strict
    conda create -n eantic libeantic pyeantic
    conda activate eantic

These stable builds are maintained by [conda-forge](https://github.com/conda-forge/e-antic-feedstock):

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-e--antic-green.svg)](https://anaconda.org/conda-forge/libeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/libeantic.svg)](https://anaconda.org/conda-forge/libeantic) | [![Conda Version](https://img.shields.io/conda/vn/conda-forge/libeantic.svg)](https://anaconda.org/conda-forge/libeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/conda-forge/libeantic.svg)](https://anaconda.org/conda-forge/libeantic) |
| [![Build](https://img.shields.io/badge/recipe-pyeantic-green.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Version](https://img.shields.io/conda/vn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) |

You can also install the latest (experimental) versions by replacing the third line above with

    conda create -n eantic -c flatsurf libeantic pyeantic

The latest available experimental versions are:

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-e--antic-green.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libeantic.svg)](https://anaconda.org/flatsurf/libeantic) |
| [![Build](https://img.shields.io/badge/recipe-pyeantic-green.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) |
