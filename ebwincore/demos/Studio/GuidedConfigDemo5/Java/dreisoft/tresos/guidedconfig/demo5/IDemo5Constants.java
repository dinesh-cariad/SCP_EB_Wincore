/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo5;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeColumnDescription;
import dreisoft.tresos.guidedconfig.demo5.generated.nls.MyEclipseNLS;

/**
 * Constants for Demo5.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public interface IDemo5Constants
{

    // page IDs
    public static final String PAGE_ID_MULTI = "DemoMultiPage";
    public static final String PAGE_ID_1 = "DemoSinglePage1";
    public static final String PAGE_ID_2 = "DemoSinglePage2";
    public static final String PAGE_ID_3 = "DemoSinglePage3";

    // common constants for all pages
    public static final String WIDGET_ID_LABEL = "label";
    public static final String MEMENTO_KEY_LABEL = WIDGET_ID_LABEL;

    public static final String WIDGET_ID_GROUP = "Group";

    // page 1
    public static final String WIDGET_ID_TEXT = "text";
    public static final String MEMENTO_KEY_TEXT = WIDGET_ID_TEXT;

    public static final String WIDGET_ID_FLOAT = "float";
    public static final String MEMENTO_KEY_FLOAT = WIDGET_ID_FLOAT;

    public static final String WIDGET_ID_TABLE = "table";
    public static final String MEMENTO_KEY_TABLE = WIDGET_ID_TABLE;

    public static final String WIDGET_ID_CHECKBOX = "checkbox";
    public static final String MEMENTO_KEY_CHECKBOX = WIDGET_ID_CHECKBOX;

    public static final String WIDGET_ID_COMBO_NUMBER = "comboNumber";
    public static final String MEMENTO_KEY_COMBO_NUMBER = WIDGET_ID_COMBO_NUMBER;

    public static final String WIDGET_ID_COMBO_TEXT = "comboText";
    public static final String MEMENTO_KEY_COMBO_TEXT = WIDGET_ID_COMBO_TEXT;

    // page 2
    public static final String WIDGET_ID_PAGE2_TEXT = "Page2Text";
    public static final String MEMENTO_KEY_PAGE2_TEXT = WIDGET_ID_PAGE2_TEXT;

    public static final String WIDGET_ID_PAGE2_COMBO = "Page2Combo";
    public static final String MEMENTO_KEY_PAGE2_COMBO = WIDGET_ID_PAGE2_COMBO;

    // page 3
    public static final String WIDGET_ID_TREE = "Tree";
    public static final String MEMENTO_KEY_TREE = WIDGET_ID_TREE;

    // Memento Ids for columns of table and treetable
    public static final String MEMENTO_COLUMN_FLOAT = "FloatColumnMemento";
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
        IDemo5Constants.MEMENTO_COLUMN_COMBO,
        IDemo5Constants.COLUMN_WIDTH,
        new String[]{"ONE", "TWO", "THREE"},
        "ONE");
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_BOOL = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_BOOL(),
        IDemo5Constants.MEMENTO_COLUMN_BOOL,
        WidgetFactory.TextFormat.BOOLEAN,
        IDemo5Constants.COLUMN_WIDTH,
        "false");
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_STRING = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_STRING(),
        IDemo5Constants.MEMENTO_COLUMN_STRING,
        WidgetFactory.TextFormat.TEXT,
        IDemo5Constants.COLUMN_WIDTH,
        "abc");
    /**
     * @since 2010.a
     */
    public static final SWTColumnDescription TREE_COLUMN_DESCRIPTION_INT = new SWTColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_INT(),
        IDemo5Constants.MEMENTO_COLUMN_INT,
        WidgetFactory.TextFormat.INTEGER,
        IDemo5Constants.COLUMN_WIDTH,
        "2");
    /**
     * @since 2010.a
     */
    public static final SWTTreeColumnDescription TREE_COLUMN_DESCRIPTION = new SWTTreeColumnDescription(
        MyEclipseNLS.LABEL_COLUMN_DESC(),
        IDemo5Constants.MEMENTO_COLUMN_DESC,
        WidgetFactory.TextFormat.TREE,
        "1.1",
        new String[]{"methpro_obj.gif", "purple.gif", "html_tag_obj.gif"},
        new boolean[]{true, true, true},
        new boolean[]{true, true, false});

}
