#...............................................................................
#
#  This file is part of the Jancy toolkit.
#
#  Jancy is distributed under the MIT license.
#  For details see accompanying license.txt file,
#  the public copy of which is also available at:
#  http://tibbo.com/downloads/archive/jancy/license.txt
#
#...............................................................................

from pygments.lexer import RegexLexer, include, bygroups, using, this, inherit, default, words
from pygments.token import Text, Comment, Operator, Keyword, Name, String, Number, Punctuation, Error
from sphinx.highlighting import lexers


class JancyLexer(RegexLexer):
    name = 'Jancy'
    aliases = ['jnc', 'jancy']
    filenames = ['*.jnc']
    mimetypes = ['text/x-jncsrc']

    tokens = {
        'whitespace': [
            (r'\n', Text),
            (r'\s+', Text),
            (r'\\\n', Text),  # line continuation
            (r'//((\n|$)|(.|\n)*?[^\\](\n|$))', Comment.Single),
            (r'/(\\\n)?[*](.|\n)*?[*](\\\n)?/', Comment.Multiline),
        ],
        'statements': [
            (r'[rR]".*"', String),
            (r'(0[xXoObBnNdD]|[fF$])?"', String, 'string'),
            (r'(0[xXoObBnNdD])?"""(.|\n)*?"""', String),
            (r"'(\\.|\\[0-7]{1,3}|\\x[a-fA-F0-9]{1,2}|[^\\\'\n])'", String.Char),
            (r'(\d+\.\d*|\.\d+|\d+)[eE][+-]?\d+', Number.Float),
            (r'(\d+\.\d*|\.\d+|\d+[fF])', Number.Float),
            (r'0[xX][0-9a-fA-F]+', Number.Hex),
            (r'0[oO][0-7]+', Number.Oct),
            (r'0[0-7]+', Number.Oct),
            (r'0[bB][01]+', Number.Bin),
            (r'0[nNdD]\d+', Number.Integer),
            (r'\d+', Number.Integer),
            (r'\*/', Error),
            (r'[~!%^&*+=|?:<>/@-]', Operator),
            (r'[{}()\[\],.;]', Punctuation),
            (words((
                'import',
                'namespace',
                'extension',
                'dynamiclib',
                'using',
                'friend',
                'public',
                'protected',
                'alignment',
                'setas',

                'typedef',
                'alias',
                'static',
                'threadlocal',
                'abstract',
                'virtual',
                'override',
                'mutable',
                'disposable',

                'unsigned',
                'bigendian',
                'const',
                'readonly',
                'cmut',
                'volatile',
                'weak',
                'thin',
                'safe',
                'unsafe',
                'errorcode',
                'cdecl',
                'stdcall',
                'jnccall',
                'thiscall',
                'array',
                'function',
                'property',
                'bindable',
                'autoget',
                'indexed',
                'multicast',
                'event',
                'reactor',

                'auto',
                'anydata',
                'void',
                'variant',
                'bool',
                'int',
                'intptr',
                'char',
                'short',
                'long',
                'float',
                'double',

                'enum',
                'struct',
                'union',
                'class',
                'opaque',
                'exposed',
                'bitflag',

                'get',
                'set',
                'preconstruct',
                'construct',
                'destruct',
                'operator',
                'postfix',

                'new',
                'delete',
                'sizeof',
                'countof',
                'offsetof',
                'typeof',
                'bindingof',
                'dynamic',

                'if',
                'else',
                'for',
                'while',
                'do',
                'break',
                'break1',
                'break2',
                'break3',
                'break4',
                'continue',
                'continue1',
                'continue2',
                'continue3',
                'continue4',
                'return',
                'switch',
                'reswitch',
                'case',
                'default',
                'once',
                'onevent',
                'try',
                'throw',
                'catch',
                'finally',
                'nestedscope',
                'assert',

                'basetype',
                'basetype1',
                'basetype2',
                'basetype3',
                'basetype4',
                'this',
                'true',
                'false',
                'null'),
                suffix=r'\b'), Keyword),
            ('[a-zA-Z_]\w*', Name),
        ],
        'root': [
            include('whitespace'),
            include('statements'),
        ],
        'string': [
            (r'"', String, '#pop'),
            (r'\\([\\abfnrtve"\']|x[a-fA-F0-9]{2,4}|'
             r'u[a-fA-F0-9]{4}|U[a-fA-F0-9]{8}|[0-7]{1,3})', String.Escape),
            (r'[^\\"\n]+', String),  # all other characters
            (r'\\\n', String),  # line continuation
            (r'\\', String),  # stray backslash
        ],
    }

    def __init__(self, **options):
        RegexLexer.__init__(self, **options)


def setup(app):
    options = {}
    options['stripnl'] = False
    options['ensurenl'] = False

    lexer = JancyLexer(**options)
    lexers['jnc'] = lexer
    lexers['jancy'] = lexer
