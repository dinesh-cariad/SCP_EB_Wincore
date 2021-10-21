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
 * The second page of the Multipage for this demo
 *
 * <p>
 * <i> Copyright (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2009.a
 */
public class Demo5SinglePage2 extends AbstractPage
{

    public Demo5SinglePage2()
    {
        super();
    }

    @Override
    public String doGetDescription()
    {
        return "single page 2 description";
    }

    @Override
    public String doGetId()
    {
        return IDemo5Constants.PAGE_ID_2;
    }

    @Override
    public String doGetTitle()
    {
        return "single page 2";
    }

    @Override
    protected void doCreateControls(Composite parent)
    {
        WidgetFactory factory = getWidgetFactory();

        SWTGroup outerGroup = factory.createGroup(parent,
                                                  IDemo5Constants.WIDGET_ID_GROUP,
                                                  WidgetFactory.GroupDecoration.TITLE,
                                                  WidgetFactory.GroupLayout.VERTICAL,
                                                  MyEclipseNLS.LABEL_GROUP());

        factory.createLabel(outerGroup,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_INT());
        factory.createText(outerGroup,
                           IDemo5Constants.WIDGET_ID_PAGE2_TEXT,
                           IDemo5Constants.MEMENTO_KEY_PAGE2_TEXT,
                           TextFormat.INTEGER);

        factory.createLabel(outerGroup,
                            IDemo5Constants.WIDGET_ID_LABEL,
                            IDemo5Constants.MEMENTO_KEY_LABEL,
                            MyEclipseNLS.LABEL_COMBO() + " " + MyEclipseNLS.LABEL_TEXT());

        String[] range = new String[]{"ONE", "TWO", "THREE"};

        factory.createCombo(outerGroup,
                            IDemo5Constants.WIDGET_ID_PAGE2_COMBO,
                            IDemo5Constants.MEMENTO_KEY_PAGE2_COMBO,
                            TextFormat.TEXT,
                            range);

        factory.layout(parent);

    }

}
