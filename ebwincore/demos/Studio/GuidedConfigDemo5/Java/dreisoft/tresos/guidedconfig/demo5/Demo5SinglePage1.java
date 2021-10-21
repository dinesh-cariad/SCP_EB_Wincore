/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo5;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TextFormat;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.demo5.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * The first page of the Multipage for this demo
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo5SinglePage1 extends AbstractPage
{

    public Demo5SinglePage1()
    {
        super();
    }

    @Override
    public String doGetDescription()
    {
        return "single page 1 description";
    }

    @Override
    public String doGetId()
    {
        return IDemo5Constants.PAGE_ID_1;
    }

    @Override
    public String doGetTitle()
    {
        return "single page 1";
    }

    @Override
    protected void doCreateControls(Composite parent)
    {

        WidgetFactory factory = getWidgetFactory();

        SWTGroup outerGroup = factory.createGroup(parent,
                                                  IDemo5Constants.WIDGET_ID_GROUP + "_1_1",
                                                  WidgetFactory.GroupDecoration.TITLE,
                                                  WidgetFactory.GroupLayout.VERTICAL,
                                                  MyEclipseNLS.LABEL_GROUP() + "1");

        factory.createLabel(outerGroup,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_INT());

        factory.createText(outerGroup,
                           IDemo5Constants.WIDGET_ID_TEXT,
                           IDemo5Constants.MEMENTO_KEY_TEXT,
                           TextFormat.INTEGER);

        factory.createLabel(outerGroup,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_TEXT());

        String[] range = new String[]{"ONE", "TWO", "THREE"};

        factory.createCombo(outerGroup,
                            IDemo5Constants.WIDGET_ID_COMBO_TEXT,
                            IDemo5Constants.MEMENTO_KEY_COMBO_TEXT,
                            TextFormat.TEXT,
                            range);

        factory.createLabel(outerGroup,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_INT());

        range = new String[]{"1.1", "2.2", "30000.3"};
        factory.createCombo(outerGroup,
                            IDemo5Constants.WIDGET_ID_COMBO_NUMBER,
                            IDemo5Constants.MEMENTO_KEY_COMBO_NUMBER,
                            TextFormat.FLOAT,
                            range);

        SWTGroup group = factory.createGroup(parent,
                                             IDemo5Constants.WIDGET_ID_GROUP + "_1_2",
                                             WidgetFactory.GroupDecoration.TITLE,
                                             WidgetFactory.GroupLayout.VERTICAL,
                                             MyEclipseNLS.LABEL_GROUP() + "2");

        factory.createLabel(group,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_FLOAT());

        factory.createText(group,
                           IDemo5Constants.WIDGET_ID_FLOAT,
                           IDemo5Constants.MEMENTO_KEY_FLOAT,
                           TextFormat.FLOAT);

        factory.createLabel(group,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_BOOL() + "1");

        factory.createCheckBox(group, IDemo5Constants.WIDGET_ID_CHECKBOX, IDemo5Constants.MEMENTO_KEY_CHECKBOX);

        factory.createSpacer(group, 2, 1);

        factory.createLabel(group,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_BOOL() + "2");

        factory.createCheckBox(group, IDemo5Constants.WIDGET_ID_CHECKBOX + "1", IDemo5Constants.MEMENTO_KEY_CHECKBOX
            + "1");

        factory.layout(parent);
    }

}
