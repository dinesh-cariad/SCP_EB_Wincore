/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo5;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TreeTableFlags;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTColumnDescription;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTTreeTable;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.demo5.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * The last page of the Multipage for this demo.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo5SinglePage3 extends AbstractPage
{

    public Demo5SinglePage3()
    {
        super();
    }

    @Override
    public String doGetDescription()
    {
        return "single page 3 description";
    }

    @Override
    public String doGetId()
    {
        return IDemo5Constants.PAGE_ID_3;
    }

    @Override
    public String doGetTitle()
    {
        return "single page 3";
    }

    @Override
    protected void doCreateControls(Composite parent)
    {
        WidgetFactory factory = getWidgetFactory();

        SWTGroup group1 = factory.createGroup(parent,
                                              IDemo5Constants.WIDGET_ID_GROUP + "_3_1",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "1");

        //
        // TreeTable
        SWTColumnDescription[] colDescs = new SWTColumnDescription[]{
            IDemo5Constants.TREE_COLUMN_DESCRIPTION,
            IDemo5Constants.TREE_COLUMN_DESCRIPTION_INT,
            IDemo5Constants.TREE_COLUMN_DESCRIPTION_STRING,
            IDemo5Constants.TREE_COLUMN_DESCRIPTION_BOOL,
            IDemo5Constants.TREE_COLUMN_DESCRIPTION_COMBO};

        // create the TreeTable
        SWTTreeTable tree = factory.createTreeTable(group1,
                                                    IDemo5Constants.WIDGET_ID_TREE,
                                                    IDemo5Constants.MEMENTO_KEY_TREE,
                                                    MyEclipseNLS.LABEL_TREE(),
                                                    10,
                                                    colDescs,
                                                    new TreeTableFlags[]{
                                                        TreeTableFlags.ACTION_ALL,
                                                        TreeTableFlags.HIDE_DEFAULT_ICONS,
                                                        TreeTableFlags.HIDE_COLUMN_TYPE_ICON});
        // disable visibility of gridlines
        tree.setLinesVisible(false);

        // layout must be called at the end
        factory.layout(parent);
    }

}
