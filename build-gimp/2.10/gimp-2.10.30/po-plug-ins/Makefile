# Makefile for program source directory in GNU NLS utilities package.
# Copyright (C) 1995, 1996, 1997 by Ulrich Drepper <drepper@gnu.ai.mit.edu>
#
# This file file be copied and used freely without restrictions.  It can
# be used in projects which are not available under the GNU Public License
# but which still want to provide support for the GNU gettext functionality.
# Please note that the actual code is *not* freely available.
#
# - Modified by Owen Taylor <otaylor@redhat.com> to use GETTEXT_PACKAGE
#   instead of PACKAGE and to look for po2tbl in ./ not in intl/
#
# - Modified by jacob berkman <jacob@ximian.com> to install
#   Makefile.in.in and po2tbl.sed.in for use with glib-gettextize
#
# - Modified by Rodney Dawes <dobey@novell.com> for use with intltool
#
# We have the following line for use by intltoolize:
# INTLTOOL_MAKEFILE

GETTEXT_PACKAGE = gimp20-std-plug-ins
XGETTEXT_KEYWORDS=--keyword=_ --keyword=N_ --keyword=Q_:1g --keyword=C_:1c,2 --keyword=NC_:1c,2

PACKAGE = gimp
VERSION = 2.10.30

SHELL = /bin/sh

srcdir = .
top_srcdir = ..
top_builddir = ..


prefix = /usr
exec_prefix = ${prefix}
datadir = ${datarootdir}
datarootdir = ${prefix}/share
libdir = ${prefix}/lib/i386-linux-gnu
DATADIRNAME = share
itlocaledir = $(prefix)/$(DATADIRNAME)/locale
subdir = po-plug-ins
install_sh = ${SHELL} /home/osboxes/build/gimp/2.10/gimp-2.10.30/install-sh
# Automake >= 1.8 provides $(MKDIR_P).
# Until it can be supposed, use the safe fallback:
mkdir_p = $(install_sh) -d

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644

GMSGFMT = /usr/bin/msgfmt
MSGFMT = /usr/bin/msgfmt
XGETTEXT = /usr/bin/xgettext
INTLTOOL_UPDATE = /usr/bin/intltool-update
INTLTOOL_EXTRACT = /usr/bin/intltool-extract
MSGMERGE = INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --dist
GENPOT   = INTLTOOL_EXTRACT=$(INTLTOOL_EXTRACT) srcdir=$(srcdir) $(INTLTOOL_UPDATE) --gettext-package $(GETTEXT_PACKAGE) --pot

ALL_LINGUAS = 

PO_LINGUAS=$(shell if test -r $(srcdir)/LINGUAS; then grep -v "^\#" $(srcdir)/LINGUAS; fi)

USER_LINGUAS=$(shell if test -n "$(LINGUAS)"; then LLINGUAS="$(LINGUAS)"; ALINGUAS="$(ALL_LINGUAS)"; for lang in $$LLINGUAS; do if test -n "`grep ^$$lang$$ $(srcdir)/LINGUAS`" -o -n "`echo $$ALINGUAS|grep ' ?$$lang ?'`"; then printf "$$lang "; fi; done; fi)

USE_LINGUAS=$(shell if test -n "$(USER_LINGUAS)"; then LLINGUAS="$(USER_LINGUAS)"; else if test -n "$(PO_LINGUAS)"; then LLINGUAS="$(PO_LINGUAS)"; else LLINGUAS="$(ALL_LINGUAS)"; fi; fi; for lang in $$LLINGUAS; do printf "$$lang "; done)

POFILES=$(shell LINGUAS="$(USE_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.po "; done)

DISTFILES = Makefile.in.in POTFILES.in $(POFILES)
EXTRA_DISTFILES = POTFILES.skip Makevars LINGUAS

POTFILES = \
	../plug-ins/common/align-layers.c \
	../plug-ins/common/animation-optimize.c \
	../plug-ins/common/animation-play.c \
	../plug-ins/common/blinds.c \
	../plug-ins/common/blur.c \
	../plug-ins/common/border-average.c \
	../plug-ins/common/busy-dialog.c \
	../plug-ins/common/cartoon.c \
	../plug-ins/common/checkerboard.c \
	../plug-ins/common/cml-explorer.c \
	../plug-ins/common/color-cube-analyze.c \
	../plug-ins/common/color-enhance.c \
	../plug-ins/common/colorify.c \
	../plug-ins/common/colormap-remap.c \
	../plug-ins/common/compose.c \
	../plug-ins/common/contrast-retinex.c \
	../plug-ins/common/crop-zealous.c \
	../plug-ins/common/curve-bend.c \
	../plug-ins/common/decompose.c \
	../plug-ins/common/depth-merge.c \
	../plug-ins/common/despeckle.c \
	../plug-ins/common/destripe.c \
	../plug-ins/common/edge-dog.c \
	../plug-ins/common/emboss.c \
	../plug-ins/common/file-aa.c \
	../plug-ins/common/file-cel.c \
	../plug-ins/common/file-csource.c \
	../plug-ins/common/file-compressor.c \
	../plug-ins/common/file-desktop-link.c \
	../plug-ins/common/file-dicom.c \
	../plug-ins/common/file-gbr.c \
	../plug-ins/common/file-gegl.c \
	../plug-ins/common/file-gif-load.c \
	../plug-ins/common/file-gif-save.c \
	../plug-ins/common/file-gih.c \
	../plug-ins/common/file-glob.c \
	../plug-ins/common/file-header.c \
	../plug-ins/common/file-heif.c \
	../plug-ins/common/file-html-table.c \
	../plug-ins/common/file-jp2-load.c \
	../plug-ins/common/file-mng.c \
	../plug-ins/common/file-pat.c \
	../plug-ins/common/file-pcx.c \
	../plug-ins/common/file-pdf-load.c \
	../plug-ins/common/file-pdf-save.c \
	../plug-ins/common/file-pix.c \
	../plug-ins/common/file-png.c \
	../plug-ins/common/file-pnm.c \
	../plug-ins/common/file-ps.c \
	../plug-ins/common/file-psp.c \
	../plug-ins/common/file-raw-data.c \
	../plug-ins/common/file-sunras.c \
	../plug-ins/common/file-svg.c \
	../plug-ins/common/file-tga.c \
	../plug-ins/common/file-wmf.c \
	../plug-ins/common/file-xbm.c \
	../plug-ins/common/file-xmc.c \
	../plug-ins/common/file-xpm.c \
	../plug-ins/common/file-xwd.c \
	../plug-ins/common/film.c \
	../plug-ins/common/filter-pack.c \
	../plug-ins/common/fractal-trace.c \
	../plug-ins/common/goat-exercise.c \
	../plug-ins/common/gradient-map.c \
	../plug-ins/common/grid.c \
	../plug-ins/common/guillotine.c \
	../plug-ins/common/hot.c \
	../plug-ins/common/jigsaw.c \
	../plug-ins/common/mail.c \
	../plug-ins/common/max-rgb.c \
	../plug-ins/common/nl-filter.c \
	../plug-ins/common/photocopy.c \
	../plug-ins/common/plugin-browser.c \
	../plug-ins/common/procedure-browser.c \
	../plug-ins/common/qbist.c \
	../plug-ins/common/sample-colorize.c \
	../plug-ins/common/sharpen.c \
	../plug-ins/common/smooth-palette.c \
	../plug-ins/common/softglow.c \
	../plug-ins/common/sparkle.c \
	../plug-ins/common/sphere-designer.c \
	../plug-ins/common/tile.c \
	../plug-ins/common/tile-small.c \
	../plug-ins/common/unit-editor.c \
	../plug-ins/common/van-gogh-lic.c \
	../plug-ins/common/warp.c \
	../plug-ins/common/wavelet-decompose.c \
	../plug-ins/common/web-browser.c \
	../plug-ins/common/web-page.c \
	../plug-ins/file-bmp/bmp-load.c \
	../plug-ins/file-bmp/bmp-save.c \
	../plug-ins/file-bmp/bmp.c \
	../plug-ins/file-dds/dds.c \
	../plug-ins/file-dds/ddsread.c \
	../plug-ins/file-dds/ddswrite.c \
	../plug-ins/file-exr/file-exr.c \
	../plug-ins/file-faxg3/faxg3.c \
	../plug-ins/file-fits/fits.c \
	../plug-ins/file-fli/fli-gimp.c \
	../plug-ins/file-ico/ico-dialog.c \
	../plug-ins/file-ico/ico-load.c \
	../plug-ins/file-ico/ico-save.c \
	../plug-ins/file-ico/ico.c \
	../plug-ins/file-jpeg/jpeg-load.c \
	../plug-ins/file-jpeg/jpeg-save.c \
	../plug-ins/file-jpeg/jpeg.c \
	../plug-ins/file-psd/psd-image-res-load.c \
	../plug-ins/file-psd/psd-load.c \
	../plug-ins/file-psd/psd-save.c \
	../plug-ins/file-psd/psd-thumb-load.c \
	../plug-ins/file-psd/psd-util.c \
	../plug-ins/file-psd/psd.c \
	../plug-ins/file-raw/file-darktable.c \
	../plug-ins/file-raw/file-raw-formats.h \
	../plug-ins/file-raw/file-raw-placeholder.c \
	../plug-ins/file-raw/file-rawtherapee.c \
	../plug-ins/file-sgi/sgi.c \
	../plug-ins/file-tiff/file-tiff.c \
	../plug-ins/file-tiff/file-tiff-load.c \
	../plug-ins/file-tiff/file-tiff-save.c \
	../plug-ins/file-webp/file-webp.c \
	../plug-ins/file-webp/file-webp-dialog.c \
	../plug-ins/file-webp/file-webp-load.c \
	../plug-ins/file-webp/file-webp-save.c \
	../plug-ins/flame/flame.c \
	../plug-ins/fractal-explorer/fractal-explorer-dialogs.c \
	../plug-ins/fractal-explorer/fractal-explorer.c \
	../plug-ins/gfig/gfig-bezier.c \
	../plug-ins/gfig/gfig-dialog.c \
	../plug-ins/gfig/gfig-dobject.c \
	../plug-ins/gfig/gfig-grid.c \
	../plug-ins/gfig/gfig-poly.c \
	../plug-ins/gfig/gfig-preview.c \
	../plug-ins/gfig/gfig-spiral.c \
	../plug-ins/gfig/gfig-star.c \
	../plug-ins/gfig/gfig-stock.c \
	../plug-ins/gfig/gfig.c \
	../plug-ins/gimpressionist/brush.c \
	../plug-ins/gimpressionist/color.c \
	../plug-ins/gimpressionist/general.c \
	../plug-ins/gimpressionist/gimp.c \
	../plug-ins/gimpressionist/gimpressionist.c \
	../plug-ins/gimpressionist/orientation.c \
	../plug-ins/gimpressionist/orientmap.c \
	../plug-ins/gimpressionist/paper.c \
	../plug-ins/gimpressionist/placement.c \
	../plug-ins/gimpressionist/ppmtool.c \
	../plug-ins/gimpressionist/presets.c \
	../plug-ins/gimpressionist/preview.c \
	../plug-ins/gimpressionist/repaint.c \
	../plug-ins/gimpressionist/size.c \
	../plug-ins/gimpressionist/sizemap.c \
	../plug-ins/gimpressionist/utils.c \
	../plug-ins/ui/plug-in-file-gif.ui \
	../plug-ins/ui/plug-in-file-png.ui \
	../plug-ins/ui/plug-in-file-raw.ui \
	../plug-ins/ui/plug-in-file-tiff.ui \
	../plug-ins/ui/plug-in-metadata-editor.ui \
	../plug-ins/ui/plug-in-metadata-viewer.ui \
	../plug-ins/gradient-flare/gradient-flare.c \
	../plug-ins/help-browser/dialog.c \
	../plug-ins/help-browser/help-browser.c \
	../plug-ins/help/gimphelpdomain.c \
	../plug-ins/help/gimphelplocale.c \
	../plug-ins/ifs-compose/ifs-compose.c \
	../plug-ins/imagemap/imap_about.c \
	../plug-ins/imagemap/imap_browse.c \
	../plug-ins/imagemap/imap_circle.c \
	../plug-ins/imagemap/imap_cmd_clear.c \
	../plug-ins/imagemap/imap_cmd_copy.c \
	../plug-ins/imagemap/imap_cmd_copy_object.c \
	../plug-ins/imagemap/imap_cmd_create.c \
	../plug-ins/imagemap/imap_cmd_cut.c \
	../plug-ins/imagemap/imap_cmd_cut_object.c \
	../plug-ins/imagemap/imap_cmd_delete.c \
	../plug-ins/imagemap/imap_cmd_delete_point.c \
	../plug-ins/imagemap/imap_cmd_edit_object.c \
	../plug-ins/imagemap/imap_cmd_gimp_guides.c \
	../plug-ins/imagemap/imap_cmd_guides.c \
	../plug-ins/imagemap/imap_cmd_insert_point.c \
	../plug-ins/imagemap/imap_cmd_move.c \
	../plug-ins/imagemap/imap_cmd_move_down.c \
	../plug-ins/imagemap/imap_cmd_move_sash.c \
	../plug-ins/imagemap/imap_cmd_move_selected.c \
	../plug-ins/imagemap/imap_cmd_move_to_front.c \
	../plug-ins/imagemap/imap_cmd_move_up.c \
	../plug-ins/imagemap/imap_cmd_object_down.c \
	../plug-ins/imagemap/imap_cmd_object_move.c \
	../plug-ins/imagemap/imap_cmd_object_up.c \
	../plug-ins/imagemap/imap_cmd_paste.c \
	../plug-ins/imagemap/imap_cmd_select.c \
	../plug-ins/imagemap/imap_cmd_select_all.c \
	../plug-ins/imagemap/imap_cmd_select_next.c \
	../plug-ins/imagemap/imap_cmd_select_prev.c \
	../plug-ins/imagemap/imap_cmd_select_region.c \
	../plug-ins/imagemap/imap_cmd_send_to_back.c \
	../plug-ins/imagemap/imap_cmd_unselect.c \
	../plug-ins/imagemap/imap_cmd_unselect_all.c \
	../plug-ins/imagemap/imap_default_dialog.c \
	../plug-ins/imagemap/imap_edit_area_info.c \
	../plug-ins/imagemap/imap_file.c \
	../plug-ins/imagemap/imap_grid.c \
	../plug-ins/imagemap/imap_main.c \
	../plug-ins/imagemap/imap_menu.c \
	../plug-ins/imagemap/imap_object_popup.c \
	../plug-ins/imagemap/imap_polygon.c \
	../plug-ins/imagemap/imap_preferences.c \
	../plug-ins/imagemap/imap_rectangle.c \
	../plug-ins/imagemap/imap_selection.c \
	../plug-ins/imagemap/imap_settings.c \
	../plug-ins/imagemap/imap_source.c \
	../plug-ins/imagemap/imap_stock.c \
	../plug-ins/lighting/lighting-apply.c \
	../plug-ins/lighting/lighting-main.c \
	../plug-ins/lighting/lighting-ui.c \
	../plug-ins/map-object/map-object-apply.c \
	../plug-ins/map-object/map-object-main.c \
	../plug-ins/map-object/map-object-ui.c \
	../plug-ins/metadata/metadata-editor.c \
	../plug-ins/metadata/metadata-tags.c \
	../plug-ins/metadata/metadata-viewer.c \
	../plug-ins/pagecurl/pagecurl.c \
	../plug-ins/print/print-draw-page.c \
	../plug-ins/print/print-page-layout.c \
	../plug-ins/print/print.c \
	../plug-ins/screenshot/screenshot.c \
	../plug-ins/screenshot/screenshot-win32.c \
	../plug-ins/screenshot/screenshot-x11.c \
	../plug-ins/selection-to-path/pxl-outline.c \
	../plug-ins/selection-to-path/selection-to-path.c \
	../plug-ins/selection-to-path/selection-to-path-dialog.c \
	../plug-ins/twain/twain.c

CATALOGS=$(shell LINGUAS="$(USE_LINGUAS)"; for lang in $$LINGUAS; do printf "$$lang.gmo "; done)

.SUFFIXES:
.SUFFIXES: .po .pox .gmo .mo .msg .cat

.po.pox:
	$(MAKE) $(GETTEXT_PACKAGE).pot
	$(MSGMERGE) $< $(GETTEXT_PACKAGE).pot -o $*.pox

.po.mo:
	$(MSGFMT) -o $@ $<

.po.gmo:
	file=`echo $* | sed 's,.*/,,'`.gmo \
	  && rm -f $$file && $(GMSGFMT) -o $$file $<

.po.cat:
	sed -f ../intl/po2msg.sed < $< > $*.msg \
	  && rm -f $@ && gencat $@ $*.msg


all: all-yes

all-yes: $(CATALOGS)
all-no:

$(GETTEXT_PACKAGE).pot: $(POTFILES)
	$(GENPOT)

install: install-data
install-data: install-data-yes
install-data-no: all
install-data-yes: all
	$(mkdir_p) $(DESTDIR)$(itlocaledir)
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  dir=$(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES; \
	  $(mkdir_p) $$dir; \
	  if test -r $$lang.gmo; then \
	    $(INSTALL_DATA) $$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $$lang.gmo as $$dir/$(GETTEXT_PACKAGE).mo"; \
	  else \
	    $(INSTALL_DATA) $(srcdir)/$$lang.gmo $$dir/$(GETTEXT_PACKAGE).mo; \
	    echo "installing $(srcdir)/$$lang.gmo as" \
		 "$$dir/$(GETTEXT_PACKAGE).mo"; \
	  fi; \
	  if test -r $$lang.gmo.m; then \
	    $(INSTALL_DATA) $$lang.gmo.m $$dir/$(GETTEXT_PACKAGE).mo.m; \
	    echo "installing $$lang.gmo.m as $$dir/$(GETTEXT_PACKAGE).mo.m"; \
	  else \
	    if test -r $(srcdir)/$$lang.gmo.m ; then \
	      $(INSTALL_DATA) $(srcdir)/$$lang.gmo.m \
		$$dir/$(GETTEXT_PACKAGE).mo.m; \
	      echo "installing $(srcdir)/$$lang.gmo.m as" \
		   "$$dir/$(GETTEXT_PACKAGE).mo.m"; \
	    else \
	      true; \
	    fi; \
	  fi; \
	done

# Empty stubs to satisfy archaic automake needs
dvi info tags TAGS ID:

# Define this as empty until I found a useful application.
install-exec installcheck:

uninstall:
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo; \
	  rm -f $(DESTDIR)$(itlocaledir)/$$lang/LC_MESSAGES/$(GETTEXT_PACKAGE).mo.m; \
	done

check: all $(GETTEXT_PACKAGE).pot
	rm -f missing notexist
	srcdir=$(srcdir) $(INTLTOOL_UPDATE) -m
	if [ -r missing -o -r notexist ]; then \
	  exit 1; \
	fi

mostlyclean:
	rm -f *.pox $(GETTEXT_PACKAGE).pot *.old.po cat-id-tbl.tmp
	rm -f .intltool-merge-cache

clean: mostlyclean

distclean: clean
	rm -f Makefile Makefile.in POTFILES stamp-it
	rm -f *.mo *.msg *.cat *.cat.m *.gmo
	# Around December of 2020, a possible change on intltool-merge or some
	# other dependency in Debian testing broke the CI distcheck job, leaving
	# some lock files. This CLEANFILES rule should be removed once this is
	# fixed upstream.
	rm -f .intltool-merge-cache.lock

maintainer-clean: distclean
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	rm -f Makefile.in.in

distdir = ../$(PACKAGE)-$(VERSION)/$(subdir)
dist distdir: $(DISTFILES)
	dists="$(DISTFILES)"; \
	extra_dists="$(EXTRA_DISTFILES)"; \
	for file in $$extra_dists; do \
	  test -f $(srcdir)/$$file && dists="$$dists $(srcdir)/$$file"; \
	done; \
	for file in $$dists; do \
	  test -f $$file || file="$(srcdir)/$$file"; \
	  ln $$file $(distdir) 2> /dev/null \
	    || cp -p $$file $(distdir); \
	done

update-po: Makefile
	$(MAKE) $(GETTEXT_PACKAGE).pot
	tmpdir=`pwd`; \
	linguas="$(USE_LINGUAS)"; \
	for lang in $$linguas; do \
	  echo "$$lang:"; \
	  result="`$(MSGMERGE) -o $$tmpdir/$$lang.new.po $$lang`"; \
	  if $$result; then \
	    if cmp $(srcdir)/$$lang.po $$tmpdir/$$lang.new.po >/dev/null 2>&1; then \
	      rm -f $$tmpdir/$$lang.new.po; \
            else \
	      if mv -f $$tmpdir/$$lang.new.po $$lang.po; then \
	        :; \
	      else \
	        echo "msgmerge for $$lang.po failed: cannot move $$tmpdir/$$lang.new.po to $$lang.po" 1>&2; \
	        rm -f $$tmpdir/$$lang.new.po; \
	        exit 1; \
	      fi; \
	    fi; \
	  else \
	    echo "msgmerge for $$lang.gmo failed!"; \
	    rm -f $$tmpdir/$$lang.new.po; \
	  fi; \
	done

Makefile POTFILES: stamp-it
	@if test ! -f $@; then \
	  rm -f stamp-it; \
	  $(MAKE) stamp-it; \
	fi

stamp-it: Makefile.in.in $(top_builddir)/config.status POTFILES.in
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/Makefile.in CONFIG_HEADERS= CONFIG_LINKS= \
	       $(SHELL) ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
