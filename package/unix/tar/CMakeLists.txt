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

axl_configure_file_w_permissions (
	build-tar-src.sh.in
	build-tar-src.sh
	OWNER_READ OWNER_WRITE OWNER_EXECUTE
	GROUP_READ GROUP_WRITE GROUP_EXECUTE
	WORLD_READ WORLD_WRITE WORLD_EXECUTE
	)

axl_configure_file_w_permissions (
	build-tar-bin.sh.in
	build-tar-bin.sh
	OWNER_READ OWNER_WRITE OWNER_EXECUTE
	GROUP_READ GROUP_WRITE GROUP_EXECUTE
	WORLD_READ WORLD_WRITE WORLD_EXECUTE
	)

#...............................................................................
