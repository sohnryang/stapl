# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information
import os
import subprocess

project = "stapl"
copyright = "2022, sohnryang"
author = "sohnryang"
release = "0.0.0"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ["breathe"]
breathe_default_project = "Stapl"

templates_path = ["_templates"]
exclude_patterns = ["_build", "Thumbs.db", ".DS_Store"]


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = "furo"
html_title = "stapl"
html_theme_options = {
    "source_repository": "https://github.com/sohnryang/stapl",
    "source_branch": "main",
}
html_static_path = ["_static"]


def configure_doxyfile(input_dir, output_dir):
    with open("Doxyfile.in") as f:
        filedata = f.read()
    replace_table = {
        "@CMAKE_PROJECT_NAME@": "Stapl",
        "@DOXYGEN_OUTPUT_DIR@": output_dir,
        "@DOXYGEN_INPUT_DIR@": input_dir,
    }
    for pattern, value in replace_table.items():
        filedata.replace(pattern, value)
    with open("Doxyfile", "w") as f:
        f.write(filedata)


rtd_build = os.environ.get("READTHEDOCS", None) == "True"
breathe_projects = {}
if rtd_build:
    input_dir = "../include"
    output_dir = "build"
    configure_doxyfile(input_dir, output_dir)
    subprocess.call("doxygen", shell=True)
    breathe_projects["Stapl"] = output_dir + "/xml"
