/*
 * Copyright (C) Elektrobit Automotive GmbH
 * All rights reserved
 */

package dreisoft.tresos.guidedconfig.demo6;

import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory;
import dreisoft.tresos.guidedconfig.api.gui.WidgetFactory.TextFormat;
import dreisoft.tresos.guidedconfig.api.gui.form.SWTGroup;
import dreisoft.tresos.guidedconfig.api.gui.page.AbstractPage;
import dreisoft.tresos.guidedconfig.demo6.generated.nls.MyEclipseNLS;

import org.eclipse.swt.widgets.Composite;

/**
 * Page for demo6, that is only shown if the project provides system model support.
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo6Page extends AbstractPage
{

    public static final String ID = "Demo6Page";

    public Demo6Page()
    {
        super();
    }

    @Override
    public String doGetDescription()
    {
        return MyEclipseNLS.DESCRIPTION_PAGE();
    }

    @Override
    public String doGetId()
    {
        return ID;
    }

    @Override
    public String doGetTitle()
    {
        return MyEclipseNLS.TITLE_PAGE();
    }

    @Override
    protected void doCreateControls(Composite parent)
    {

        WidgetFactory factory = getWidgetFactory();

        SWTGroup group1 = factory.createGroup(parent,
                                              IDemo6Constants.WIDGET_ID_GROUP + "_1_1",
                                              WidgetFactory.GroupDecoration.TITLE,
                                              WidgetFactory.GroupLayout.VERTICAL,
                                              MyEclipseNLS.LABEL_GROUP() + "1");

        factory.createLabel(group1,
                            IDemo6Constants.WIDGET_ID_LABEL,
                            IDemo6Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_INT());

        factory.createText(group1,
                           IDemo6Constants.WIDGET_ID_TEXT,
                           IDemo6Constants.MEMENTO_KEY_TEXT,
                           TextFormat.INTEGER);

        factory.createLabel(group1,
                            IDemo6Constants.WIDGET_ID_LABEL,
                            IDemo6Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_TEXT());

        String[] range = new String[]{"ONE", "TWO", "THREE"};

        factory.createCombo(group1,
                            IDemo6Constants.WIDGET_ID_COMBO_TEXT,
                            IDemo6Constants.MEMENTO_KEY_COMBO_TEXT,
                            TextFormat.TEXT,
                            range);

        factory.createLabel(group1,
                            IDemo6Constants.WIDGET_ID_LABEL,
                            IDemo6Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_INT());

        range = new String[]{"1.1", "2.2", "30000.3"};
        factory.createCombo(group1,
                            IDemo6Constants.WIDGET_ID_COMBO_NUMBER,
                            IDemo6Constants.MEMENTO_KEY_COMBO_NUMBER,
                            TextFormat.FLOAT,
                            range);

        factory.layout(parent);
    }

}
