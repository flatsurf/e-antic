# Install e-antic in Conda

You can install this package with conda. Download and install
[Miniconda](https://conda.io/miniconda.html), then run

    conda config --add channels conda-forge
    conda config --set channel_priority strict
    conda create -n eantic e-antic pyeantic
    conda activate eantic

These stable builds are maintained by [conda-forge](https://github.com/conda-forge/e-antic-feedstock):

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-e--antic-green.svg)](https://anaconda.org/conda-forge/e-antic) | [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/e-antic.svg)](https://anaconda.org/conda-forge/e-antic) | [![Conda Version](https://img.shields.io/conda/vn/conda-forge/e-antic.svg)](https://anaconda.org/conda-forge/e-antic) | [![Conda Platforms](https://img.shields.io/conda/pn/conda-forge/e-antic.svg)](https://anaconda.org/conda-forge/e-antic) |
| [![Build](https://img.shields.io/badge/recipe-pyeantic-green.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Version](https://img.shields.io/conda/vn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/conda-forge/pyeantic.svg)](https://anaconda.org/conda-forge/pyeantic) |

You can also install the latest (experimental) versions by replacing the third line above with

    conda create -n eantic -c flatsurf e-antic pyeantic

The latest available experimental versions are:

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-e--antic-green.svg)](https://anaconda.org/flatsurf/e-antic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/e-antic.svg)](https://anaconda.org/flatsurf/e-antic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/e-antic.svg)](https://anaconda.org/flatsurf/e-antic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/e-antic.svg)](https://anaconda.org/flatsurf/e-antic) |
| [![Build](https://img.shields.io/badge/recipe-pyeantic-green.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyeantic.svg)](https://anaconda.org/flatsurf/pyeantic) |
