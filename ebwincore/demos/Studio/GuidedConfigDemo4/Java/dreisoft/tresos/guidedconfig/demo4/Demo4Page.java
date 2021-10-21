/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo4;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.ButtonFlags;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TextFormat;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TreeTableFlags;
import dreisoft.tresos.guidedconfig.api.gui.eventing.ButtonSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.eventing.WidgetSelectionEvent;
import dreisoft.tresos.guidedconfig.api.gui.form.*;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTable.SortFlags;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTableController.AbstractRowFilter;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.api.memento.Memento;
import dreisoft.tresos.guidedconfig.demo4.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * This single page shows all predefined widgets.
 * <p>
 * It also uses Button SelectionEvents to change the sort column of the table and one button to set a row filter.
 * </p>
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo4Page extends AbstractPage
{

    public static final String ID = "Demo4Page";

    private SWTTable m_table;
    private RowFilter m_filter;

    public Demo4Page()
    {
        super();
        m_filter = new RowFilter();
    }

    @Override
    public String doGetDescription()
    {
        return "single page with all standard widgets";
    }

    @Override
    public String doGetId()
    {
        return ID;
    }

    @Override
    public String doGetTitle()
    {
        return "Demo4 Editor with all standard widgets";
    }

    @Override
    protected void doCreateControls(Composite parent)
    {

        // retrieve the widget factory
        WidgetFactory factory = getWidgetFactory();

        // create a non collapsable group without title
        SWTGroup group0 = factory.createGroup(parent,
                                              IDemo4Constants.WIDGET_ID_GROUP + "_1_0",
                                              WidgetFactory.GroupDecoration.NONE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "0");
        // create text for the browser widget
        StringBuffer text = new StringBuffer();
        text.append("<body style='background-color:#CCFFFF'>");
        text.append("This demo shows all predefined widgets provided by the GuidedConfig API.<br/>");
        text.append("<ul><li>Textbox</li>");
        text.append("<li>ComboBox</li>");
        text.append("<li>Checkbox</li>");
        text.append("<li>Browser</li>");
        text.append("<li>Spacer</li>");
        text.append("<li>Button</li>");
        text.append("<li>Table</li>");
        text.append("<li>TreeTable</li>");
        text.append("<li>Group</li></ul>");
        text.append("</body>");

        // create a browser widget
        factory.createBrowser(group0,
                              IDemo4Constants.WIDGET_ID_BROWSER,
                              IDemo4Constants.MEMEMTO_KEY_BROWSER,
                              text.toString(),
                              4,
                              1,
                              -1,
                              200,
                              true);

        // create a collapseable group
        SWTGroup group1 = factory.createGroup(parent,
                                              IDemo4Constants.WIDGET_ID_GROUP + "_1_1",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "1");

        // add Integer Textbox with label
        factory.createLabel(group1,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_INT());
        factory.createText(group1,
                           IDemo4Constants.WIDGET_ID_TEXT,
                           IDemo4Constants.MEMENTO_KEY_TEXT,
                           TextFormat.INTEGER);

        // create text ComboBox with label
        factory.createLabel(group1,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_TEXT());
        String[] range = new String[]{"ONE", "TWO", "THREE"};
        factory.createCombo(group1,
                            IDemo4Constants.WIDGET_ID_COMBO_TEXT,
                            IDemo4Constants.MEMENTO_KEY_COMBO_TEXT,
                            TextFormat.TEXT,
                            range);

        // create float ComboBox with label
        factory.createLabel(group1,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_INT());
        range = new String[]{"1.1", "2.2", "30000.3"};
        factory.createCombo(group1,
                            IDemo4Constants.WIDGET_ID_COMBO_NUMBER,
                            IDemo4Constants.MEMENTO_KEY_COMBO_NUMBER,
                            TextFormat.FLOAT,
                            range);

        // create a collapseable group
        SWTGroup group2 = factory.createGroup(parent,
                                              IDemo4Constants.WIDGET_ID_GROUP + "_1_2",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "2");

        // create textbox with label
        factory.createLabel(group2,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_FLOAT());
        factory.createText(group2,
                           IDemo4Constants.WIDGET_ID_FLOAT,
                           IDemo4Constants.MEMENTO_KEY_FLOAT,
                           TextFormat.FLOAT);

        // create checkbox with label
        factory.createLabel(group2,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_BOOL() + "1");
        factory.createCheckBox(group2, IDemo4Constants.WIDGET_ID_CHECKBOX, IDemo4Constants.MEMENTO_KEY_CHECKBOX);

        // create spacer with span=2 so the next checkbox is layout in the next row
        factory.createSpacer(group2, 2, 1);

        // create checkbox with label
        factory.createLabel(group2,
                            IDemo4Constants.WIDGET_ID_LABEL,
                            IDemo4Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_BOOL() + "2");
        factory.createCheckBox(group2,
                               IDemo4Constants.WIDGET_ID_CHECKBOX + "1",
                               IDemo4Constants.MEMENTO_KEY_CHECKBOX + "1");

        // ////////////////////////////////////
        SWTGroup group3 = factory.createGroup(parent,
                                              IDemo4Constants.WIDGET_ID_GROUP + "_1_3",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "3");

        // create Table
        SWTColumnDescription[] columns = new SWTColumnDescription[]{
            IDemo4Constants.TABLE_COLUMN_DESCRIPTION_INT,
            IDemo4Constants.TABLE_COLUMN_DESCRIPTION_STRING,
            IDemo4Constants.TABLE_COLUMN_DESCRIPTION_BOOL,
            IDemo4Constants.TABLE_COLUMN_DESCRIPTION_COMBO,
            IDemo4Constants.TABLE_COLUMN_DESCRIPTION_FLOAT};

        // create table
        m_table = factory.createTable(group3,
                                      IDemo4Constants.WIDGET_ID_TABLE,
                                      IDemo4Constants.MEMENTO_KEY_TABLE,
                                      MyEclipseNLS.LABEL_TABLE(),
                                      10,
                                      columns);
        // create Toggle button
        SWTButton filterButton = factory.createButton(group3,
                                                      "FilterTable",
                                                      "FilterTable",
                                                      "FilterTable",
                                                      "spy.gif",
                                                      ButtonFlags.TOGGLE);

        // filter table only rows with String!= hello are shown
        RowFilterSelection filterEvent = new RowFilterSelection();
        filterButton.getController().addSelectionEventListener(filterEvent);
        m_filter.init(m_table.getController());
        filterButton.setToolTipText("Filter all rows with value 'hello' in column String.");

        // create push button
        SWTButton sortButton = factory.createButton(group3,
                                                    "ChangeSortColumn",
                                                    "ChangeSortColumn",
                                                    "ChangeSortColumn",
                                                    "spy.gif",
                                                    ButtonFlags.PUSH);
        SortColumnSelection sorting = new SortColumnSelection();
        sortButton.getController().addSelectionEventListener(sorting);
        sortButton.setToolTipText("Change sort column to column 'Text'.");

        // //////////////////////
        // create a collapseable group
        SWTGroup group4 = factory.createGroup(parent,
                                              IDemo4Constants.WIDGET_ID_GROUP + "_1_4",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "4");

        //
        // create TreeTable
        SWTColumnDescription[] colDescs = new SWTColumnDescription[]{
            IDemo4Constants.TREE_COLUMN_DESCRIPTION_DESC,
            IDemo4Constants.TREE_COLUMN_DESCRIPTION_INT,
            IDemo4Constants.TREE_COLUMN_DESCRIPTION_STRING,
            IDemo4Constants.TREE_COLUMN_DESCRIPTION_BOOL,
            IDemo4Constants.TREE_COLUMN_DESCRIPTION_COMBO};
        // Readonly column
        colDescs[1].setEditable(false);

        // create the TreeTable
        SWTTreeTable tree = factory.createTreeTable(group4,
                                                    IDemo4Constants.WIDGET_ID_TREE,
                                                    IDemo4Constants.MEMENTO_KEY_TREE,
                                                    MyEclipseNLS.LABEL_TREE(),
                                                    10,
                                                    colDescs,
                                                    new TreeTableFlags[]{
                                                        TreeTableFlags.ACTION_ALL,
                                                        TreeTableFlags.HIDE_DEFAULT_ICONS,
                                                        TreeTableFlags.HIDE_COLUMN_TYPE_ICON});
        // disable visibility of gridlines
        tree.setLinesVisible(false);

        factory.layout(parent);
    }

    // ///////////////////////
    // ROW FILTER
    // //////////////////////
    private class RowFilterSelection implements ISelectionEventListener
    {

        @Override
        public void selectionChanged(WidgetSelectionEvent event)
        {
            if (!(event instanceof ButtonSelectionEvent))
            {
                return;
            }
            ButtonSelectionEvent bse = (ButtonSelectionEvent)event;
            if (bse.getAction() == ButtonSelectionEvent.ACTION_ON)
            {
                m_table.getController().setRowFilter(m_filter);
            }
            else if (bse.getAction() == ButtonSelectionEvent.ACTION_OFF)
            {
                m_table.getController().setRowFilter(null);
            }
        }
    }

    private class RowFilter extends AbstractRowFilter
    {

        @Override
        public boolean doFilterRow(int row)
        {
            SWTTableController controller = getTableController();
            Memento rowMemento = controller.getRowMemento(row);
            if ("hello".equals(rowMemento.getString(IDemo4Constants.MEMENTO_COLUMN_STRING)))
            {
                return false;
            }
            return true;
        }
    }

    // ///////////////////////
    // CHANGE SORT COLUMN
    // //////////////////////
    private class SortColumnSelection implements ISelectionEventListener
    {

        @Override
        public void selectionChanged(WidgetSelectionEvent event)
        {
            if (!(event instanceof ButtonSelectionEvent))
            {
                return;
            }
            m_table.setSortColumn(MyEclipseNLS.LABEL_TEXT(), SortFlags.COLUMN_ORDER_ASCENDING);
        }
    }

}
