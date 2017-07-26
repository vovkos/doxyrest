#...............................................................................
#
#  This file is part of the Doxyrest toolkit.
#
#  Doxyrest is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/doxyrest/license.txt
#
#...............................................................................

import re
from docutils.parsers.rst import Directive, directives
from docutils.transforms import Transform
from docutils import nodes
from sphinx import roles, addnodes, config


class HighlightedText(nodes.General, nodes.TextElement):
    def __init__(self, *args, **kwargs):
        nodes.TextElement.__init__(self, *args, **kwargs)


def visit_highlighted_text_node(self, node):
    text_node = node.children[0]
    language = node['language']

    if language == 'none':
        self.body.append(text_node)
    else:
        options = {}
        options['stripnl'] = False
        options['ensurenl'] = False

        text = text_node

        # pygments always ensures a newline at the end and there is no way to tell
        # if the newline is originating from source or was it inserted by pygments
        # as a workaround, duplicate trailing newline and then strip it

        if text.endswith('\n'):
            text = text + '\n'

        highlighted = self.highlighter.highlight_block(text, language, options)

        match = re.search('<pre>(.*)\n</pre>', highlighted, re.DOTALL)
        if not match:
            self.body.append(text_node)
        else:
            self.body.append(match.group(1))

    raise nodes.SkipNode


class RefCodeBlock(Directive):
    has_content = True
    required_arguments = 0
    optional_arguments = 1
    final_argument_whitespace = False

    option_spec = {
        'linenos': directives.flag,
        'dedent': int,
        'lineno-start': int,
        'emphasize-lines': directives.unchanged_required,
        'caption': directives.unchanged_required,
        'class': directives.class_option,
        'name': directives.unchanged,
    }

    def __init__(self, *args, **kwargs):
        Directive.__init__(self, *args, **kwargs)
        self.re_prog = re.compile('(:c?ref:)?`((.+?)(\s*<([^<>]*)>)?)`(_+)?')

    def run(self):
        config = self.state.document.settings.env.config
        code = u'\n'.join(self.content)
        pos = 0
        node = nodes.literal_block('.', '') # single char to prevent sphinx from trying to highlight it
        node['classes'] += ['highlight']    # we are stripping pyments-generated <div>
        node['classes'] += self.options.get('class', [])

        if len(self.arguments) >= 1:
            language = self.arguments[0]
        else:
            language = config.highlight_language

        while True:
            match = self.re_prog.search(code, pos)
            if match is None:
                plain_text = code[pos:]
                if plain_text != "":
                    node += HighlightedText(plain_text, plain_text, language=language)
                break

            plain_text = code[pos:match.start()]
            if plain_text != "":
                node += HighlightedText(plain_text, plain_text, language=language)

            raw_text = match.group(0)
            role = match.group(1)
            contents = match.group(2)
            text = match.group(3)
            target = match.group(5)
            underscore = match.group(6)

            if underscore:
                ref_node = nodes.reference(raw_text, text, refuri=target if target else text)
            elif not role or role == ':cref:':
                target = 'cid-' + contents.lower ()
                ref_node = create_xref_node(raw_text, contents, target)
            else:
                ref_node = create_xref_node(raw_text, text, target)

            node += ref_node
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


def cref_role(typ, raw_text, text, lineno, inliner, options={}, content=[]):
    target = 'cid-' + text.lower()

    if text.find(' ') == -1:
        node = nodes.literal(raw_text, '')
    else:
        node = nodes.inline(raw_text, '')
        target = target.replace(' ', '-')

    node['classes'] += ['cref']

    node += create_xref_node(raw_text, text, target)

    return [node], []


def target_role(typ, raw_text, text, lineno, inliner, options={}, content=[]):
    node = nodes.target(raw_text, '')
    node.line = lineno
    node['names'] += [text]

    inliner.document.note_explicit_target(node)
    return [node], []


class RefTransform(Transform):
    default_priority = 100

    node_classes = {
        nodes.literal,
        nodes.strong,
        nodes.emphasis
    }

    def __init__(self, document, startnode=None):
        Transform.__init__(self, document, startnode)
        self.re_prog = re.compile('(:c?ref:)?`(.+?)(\s*<([^<>]*)>)?`')

    @staticmethod
    def node_filter(node):
        for node_class in RefTransform.node_classes:
            if isinstance (node, node_class):
                return node['classes'] == []

        return False

    def apply(self):
        for node in self.document.traverse(RefTransform.node_filter):
            code = node.astext()
            node.children = []

            pos = 0

            while True:
                match = self.re_prog.search(code, pos)
                if match is None:
                    plain_text = code[pos:]
                    if plain_text != "":
                        node += nodes.Text(plain_text, plain_text)
                    break

                plain_text = code[pos:match.start()]
                if plain_text != "":
                    node += nodes.Text(plain_text, plain_text)

                raw_text = match.group(0)
                role = match.group(1)
                text = match.group(2)
                target = match.group(4)

                if not role or role == ':cref:':
                    target = 'cid-' + text.lower ()

                node += create_xref_node(raw_text, text, target)
                pos = match.end()


def setup(app):
    app.add_node(
        HighlightedText,
        html=(visit_highlighted_text_node, None),
        latex=(visit_highlighted_text_node, None)
        )

    app.add_role('cref', cref_role)
    app.add_role('target', target_role)

    directives.register_directive('ref-code-block', RefCodeBlock)
    directives.register_directive('code-block', RefCodeBlock) # replace

    app.add_transform(RefTransform)
