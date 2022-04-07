{% import 'macros.rst' as macros with context -%}

{% set class = api[selector] %}

.. _{{selector}}:

{{ macros.format_heading(class["kind"] + " " + class["name"])}}

{{ macros.format_paragraphs(class["detaileddescription"]) }}
