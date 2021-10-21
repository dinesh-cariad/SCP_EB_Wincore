/*
 * Copyright (C) EB Automotive
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo9;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.ValueConverter.NumberFormat;
import dreisoft.tresos.guidedconfig.demo9.generated.nls.MyEclipseNLS;

/**
 * Defines constants used within this demo.
 *
 * @since 2008.b
 */
public interface IConstants
{

    // tree
    public static final String WIDGET_ID_TREE = "Tree";
    public static final String MEMENTO_KEY_TREE = WIDGET_ID_TREE;

    // table
    public static final String WIDGET_ID_TABLE = "Table";
    public static final String MEMENTO_KEY_TABLE = WIDGET_ID_TABLE;

    // label
    public static final String WIDGET_ID_LABEL = "Label";
    public static final String MEMENTO_KEY_LABEL = WIDGET_ID_LABEL;

    // Memento Ids for columns of table and treetable
    public static final String MEMENTO_COLUMN_INT = "IntColumnMemento";
    public static final String MEMENTO_COLUMN_STRING = "StringColumnMemento";
    public static final String MEMENTO_COLUMN_BOOL = "BooleanColumnMemento";
    public static final String MEMENTO_COLUMN_COMBO = "ComboBoxMemento";
    public static final String MEMENTO_COLUMN_DESC = "DescMemento";

    /**
     * Width in pixels of the table columns.
     */
    public static final int COLUMN_WIDTH = 100;

    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_COMBO = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_COMBO(),
        IConstants.MEMENTO_COLUMN_COMBO,
        WidgetFactory.TextFormat.TEXT,
        IConstants.COLUMN_WIDTH,
        new String[]{"ONE", "TWO", "THREE"},
        "ONE",
        NumberFormat.DEC,
        false,
        false);
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_BOOL = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_BOOL(),
        IConstants.MEMENTO_COLUMN_BOOL,
        WidgetFactory.TextFormat.BOOLEAN,
        IConstants.COLUMN_WIDTH,
        null,
        "false",
        NumberFormat.DEC,
        false,
        false);
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_STRING = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_STRING(),
        IConstants.MEMENTO_COLUMN_STRING,
        WidgetFactory.TextFormat.TEXT,
        IConstants.COLUMN_WIDTH,
        null,
        "abc",
        NumberFormat.DEC,
        false,
        false);
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_INT = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_INT(),
        IConstants.MEMENTO_COLUMN_INT,
        WidgetFactory.TextFormat.INTEGER,
        IConstants.COLUMN_WIDTH,
        null,
        "2",
        NumberFormat.DEC,
        false,
        false);
    /**
     * @since 2010.a
     */
    public static final SWTTreeColumnDescription TREE_COLUMN_DESCRIPTION = new SWTTreeColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_DESC(),
        IConstants.MEMENTO_COLUMN_DESC,
        WidgetFactory.TextFormat.TREE,
        100,
        "1.1",
        null,
        new boolean[]{false},
        new boolean[]{false});

}
