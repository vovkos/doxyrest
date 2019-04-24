--------------------------------------------------------------------------------
--
--  This file is part of the Doxyrest toolkit.
--
--  Doxyrest is distributed under the MIT license.
--  For details see accompanying license.txt file,
--  the public copy of which is also available at:
--  http://tibbo.com/downloads/archive/doxyrest/license.txt
--
--------------------------------------------------------------------------------

--!
--! Table containing a list of frame directories. All frame files will be
--! searched in directories -- and in the sequence -- specified here.
--!

FRAME_DIR_LIST = {}

--!
--! The output master (index) reStructuredText file. Usually, the index frame
--! also generates auxillary files -- they will be placed next to the master
--! file. The command line option ``-f`` *overrides* this value.
--! If neither ``FRAME_FILE`` nor ``-f`` is specified, ``index.rst.in`` will be
--! used as the default frame file.
--!

FRAME_FILE = nil

--!
--! The input master (index) XML file. Specifying it here allows calling
--! ``doxyrest`` without parameters; otherwise, the master XML *must* be passed
--! via the command line. If both ``INPUT_FILE`` and command line parameter are
--! specified, the command line takes precedence.
--!

INPUT_FILE = nil

--!
--! The output master (index) reStructuredText file. Usually, the index frame
--! also generates auxillary files -- they will be placed next to the master
--! file. The command line option ``-o`` *overrides* this value. If neither
--! ``OUTPUT_FILE`` nor ``-o`` is specified, ``index.rst`` will be used as
--! the default output master file.
--!

OUTPUT_FILE = nil

--!
--! File with project-specific reStructuredText definitions. When non``nil``,
--! this file will be included at the top of every generated ``.rst`` file.
--!

FORCE_INCLUDE_FILE = nil

--!
--! If you want to add extra reStructuredText documentation pages, do so
--! by adding them to this list.
--!

EXTRA_PAGE_LIST = {}

--!
--! The title of the main (index) page.
--!

INDEX_TITLE = "My Project Documentation"

--!
--! File with project introduction (reStructuredText). When non-nil, this file
--! will be included into ``index.rst`` file and NOT added to the list of other
--! documentation pages.
--!

INTRO_FILE = nil

--!
--! Specify the main language of your project; this string will be used for
--! the reStructuredText ``.. code::`` sections and for conditional formatting
--! of module item declarations.
--!

LANGUAGE = cpp

--!
--! Convert ``\verbatim`` sections in doxy-comments to ``.. code-block::``
--! sections in the output reStructuredText. The string value of
--! ``VERBATIM_TO_CODE_BLOCK`` will be used as the language of
--! ``.. code-block::`` section. By default, it's ``none`` which results in
--! no syntax highlighting. To disable conversion at all, use ``nil``.
--!

VERBATIM_TO_CODE_BLOCK = "none"

--!
--! If the original doxy comments contain asterisks, they have to be escaped
--! in reStructuredText (asterisks are used to mark **bold** or *italic* blocks).
--!

ESCAPE_ASTERISKS = false

--!
--! If the original doxy comments contain trailingasterisks, they have to be
--! escaped in reStructuredText (trailing underscores are used for internal
--! links).
--!

ESCAPE_TRAILING_UNDERSCORES = false

--!
--! Exclude items with higher protection level than ``PROTECTION_FILTER``:
--!
--!     1. public
--!     2. protected
--!     3. private
--!     4. package
--!
--! By default, only public items are included into documentation.
--!

PROTECTION_FILTER = "public"

--!
--! Exclude items declared in specific locations. Use a regular expression to
--! define a mask of directories/source files to completely exclude from the
--! final documentation. For example, ``.*/impl/.*lua$`` will exclude all
--! ``.lua`` files located in ``impl/`` directory.
--!

EXCLUDE_LOCATION_PATTERN = nil

--!
--! Exclude variables and functions without any documentation (no doxy-comments).
--!

EXCLUDE_UNDOCUMENTED_ITEMS = false

--!
--! Usually you don't want to include empty defines (include-guards,
--! conditional compilation switches, etc) into the project documentation.
--! Change this to ``false`` if empty defines *should* be included.
--!

EXCLUDE_EMPTY_DEFINES = true

--!
--! If non-``nil``, each define will be checked using this regular expression
--! and if its name matches, this define will be excluded from the documentation.
--!

EXCLUDE_DEFINE_PATTERN = nil

--!
--! Usually providing documentation blocks for default constructors is
--! not necessary (as to avoid redundant meaningless "Constructs a new object"
--! paragraphs). Change this to ``false`` if default constructors *should* be
--! included.
--!

EXCLUDE_DEFAULT_CONSTRUCTOR = true

--!
--! Usually providing documentation blocks for a destructors is
--! not necessary (as to avoid redundant meaningless "Destructs an object"
--! paragraphs). Change this to ``false`` if destructors *should* be
--! included.
--!

EXCLUDE_DESTRUCTORS = true

--!
--! Usually providing documentation blocks for primitive C typedefs such as:
--!
--! .. code:: C
--!
--!     typedef struct S S;
--!
--! is not necessary. Change this to ``false`` if such typedefs *should* be
--! included.
--!

EXCLUDE_PRIMITIVE_TYPEDEFS = true

--!
--! \subgroup
--!
--! Insert space between function name and parameter list like this:
--!
--! .. code:: C
--!
--!     void foo ();
--!
--! By default, ``PRE_PARAM_LIST_SPACE`` is ``false`` which yields:
--!
--! .. code:: C
--!
--!     void foo();
--!

PRE_PARAM_LIST_SPACE = false
PRE_OPERATOR_NAME_SPACE = true
PRE_OPERATOR_PARAM_LIST_SPACE = true

--!
--! Use multi-line parameter lists in function declarations if parameter count is
--! greater than this threshold. ``nil`` means don't use parameter count
--! threshold.
--!

ML_PARAM_LIST_COUNT_THRESHOLD = nil

--!
--! Use multi-line parameter lists in function declarations if single-line
--! declaration length parameter count is greater than this threshold.
--! ``nil`` means don't use length threshold.
--!

ML_PARAM_LIST_LENGTH_THRESHOLD = 80

--!
--! Use multi-line specifier-modifier lists in function declarations, i.e.
--! allocate a dedicated line for each type specifier/morifier.
--!

ML_SPECIFIER_MODIFIER_LIST = false

--!
--! Sometimes, it's required to redirect a Doxygen link to some external location.
--! In this case, add an entry to ``IMPORT_URL_MAP`` with the target URL, e.g.:
--!
--! .. code:: lua
--!
--!     IMPORT_URL_MAP =
--!     {
--!         [ "cfd9ea7a-35de-4090-a83b-3d214b3ff358/type_jnc_scheduler" ] = "https://vovkos.github.io/jancy/stdlib/class_jnc_Scheduler.html"
--!     }
--!
--! The key of the map is an 'importid' attribute. This is a non-standard Doxygen
--! attribute; Jancy compiler generates is when a referenced item is contained in an
--! imported extensions library (``.jncx``)
--!

IMPORT_URL_MAP = {}
