import re

from docutils.parsers.rst import Directive, directives
from docutils import nodes
from sphinx import addnodes


class RefCodeBlock(Directive):
    has_content = True
    required_arguments = 0
    optional_arguments = 1
    final_argument_whitespace = False
    option_spec = {}

    def __init__(self, *args, **kwargs):
        Directive.__init__(self, *args, **kwargs)
        self.re_prog = re.compile(':ref:`(.+?)\s*<([^<>]*)>`')

    def run(self):
        code = u'\n'.join(self.content)
        pos = 0
        node = nodes.literal_block('without-this-no-links-in-the-output', '')

        while True:
            match = self.re_prog.search(code, pos)
            if match is None:
                plain_text = code[pos:]
                node += nodes.Text(plain_text, plain_text)
                break

            plain_text = code[pos:match.start()]
            node += nodes.Text(plain_text, plain_text)
            node += create_xref_node(match.group(0), match.group(1), match.group(2))
            pos = match.end()

        self.add_name(node)

        return [node]


def create_xref_node(raw_text, text, target):
    node = addnodes.pending_xref(raw_text)
    node['reftype'] = 'ref'
    node['refdomain'] = 'std'
    node['reftarget'] = target
    node['refwarn'] = True
    node['refexplicit'] = True

    node += nodes.Text(text, text)

    return node


def setup(app):
    directives.register_directive('ref-code-block', RefCodeBlock)
