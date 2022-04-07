{% import 'macros.rst' as macros with context -%}


{% macro format_parameters_without_names(parameters) -%}
(
{%- for parameter in parameters -%}
    {% set type = parameter["type"] %}
    {{- macros.format_type_list(type) if loop.first else "..."-}}
    {{- ", " if not loop.last -}}
{% endfor -%}
)
{%- endmacro -%}

{# FORMAT_MEMBER_TABLE_ROW #}

{%- macro format_member_table_row(member_selector) -%}
{%- set function = api[member_selector] %}
{% if "return" in function -%}
{%- set return_type = macros.format_type_list(function["return"]["type"]) -%}
{% else %}
{%- set return_type = "" -%}
{%- endif %}
* - {{ return_type }}
  - :ref:`{{ function["name"] }}<{{member_selector}}>` {{format_parameters_without_names(function["parameters"])}}
{% endmacro -%}

{# FORMAT_MEMBER_TABLE #}

{%- macro format_member_table(member_selectors) -%}
.. list-table::
   :header-rows: 0
   :widths: auto

{% for member_selector in member_selectors %}
   {{ format_member_table_row(member_selector) | indent(width=3) }}
{%- endfor -%}

{% endmacro -%}

{%- set struct_functions = [] -%}
{%- for function_selector in api if api[function_selector]["kind"] == "function" -%}
   {%- set function = api[function_selector] -%}
   {%- if selector in function_selector -%}
      {%- do struct_functions.append(function_selector) -%}
   {%- endif -%}
{%- endfor -%}


{%- set struct = api[selector] -%}

.. _{{selector}}:

{{ macros.format_heading(struct["kind"] + " " + struct["name"])}}

{{ macros.format_paragraphs(struct["detaileddescription"]) }}


.. contents:: Table of Contents
   :local:

Table with all :ref:`{{selector}}` functions:

{{format_member_table(struct_functions)}}

-----

{% for function_selector in struct_functions %}
   {{ macros.format_function(function_selector) }}

{{ "-----" if not loop.last }}

{% endfor -%}
