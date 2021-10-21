package test.tresos.plugin.generator;

import dreisoft.tresos.datamodel2.api.model.DCtxt;
import dreisoft.tresos.generator.api.generator.CodeGeneratorContext;
import dreisoft.tresos.generator.api.generator.GeneratorWriter;
import dreisoft.tresos.generator.api.generator.ICodeGenerator2;
import dreisoft.tresos.generator.api.generator.ParameterSet;
import dreisoft.tresos.lib2.api.log.APIOperationStatus;

import java.io.IOException;

/**
 * Actual Generator implementation.
 *
 * <p>
 * See plugin.xml to find out how to register this GeneratorClass
 * </p>
 *
 * <p>
 * <i> (C) Elektrobit Automotive GmbH - All rights reserved</i>
 * </p>
 *
 * @since 2008.b
 */
public class TestGenerator implements ICodeGenerator2
{

    /**
     * Default constructor.
     *
     * @since 2008.b
     */
    public TestGenerator()
    {
    }

    /**
     * Called to run the generator.
     *
     * @see dreisoft.tresos.generator.api.generator.ICodeGenerator#generate(dreisoft.tresos.generator.api.generator.CodeGeneratorContext)
     * @since 2008.b
     */
    @Override
    public void generate(CodeGeneratorContext context)
    {
        // implement your generator here

        // Get a writer object, to be able to generate files.
        GeneratorWriter gw = null;
        try
        {
            // creates a file in the corresponding output directory
            gw = context.getWriter("TestFile.txt");
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return;
        }
        // get the corresponding DCtxt variable to gain access to the DataModel
        DCtxt ctxt = context.getDCtxtVariable();
        // ask for some values of the configuration
        long intValue = ctxt.var.getInt("ExamplesCommon/Int");
        boolean boolValue = ctxt.var.getBool("ExamplesCommon/Bool");
        double floatValue = 0.0;
        if (boolValue)
        {
            floatValue = ctxt.var.getFloat("ExamplesCommon/Float");
        }
        String stringValue = ctxt.var.getString("ExamplesCommon/String");

        // print the values to the generated file
        String lineBreak = System.getProperty("line.separator");
        gw.write("Integer Value = " + Long.toString(intValue) + lineBreak);
        gw.write("Float Value   = " + Double.toString(floatValue) + lineBreak);
        gw.write("String Value  = " + stringValue + lineBreak);
        gw.close();
    }

    /**
     * The modes in which the generator can run.
     *
     * @see dreisoft.tresos.generator.api.generator.ICodeGenerator#getAvailableModes(dreisoft.tresos.generator.api.generator.ParameterSet)
     * @since 2008.b
     */
    @Override
    public String[] getAvailableModes(ParameterSet parameters)
    {
        return new String[]{MODE_GENERATE};
        // or new String[]{MODE_GENERATE, MODE_VERIFY} if you also want to verify with this generator
    }

    /**
     * The mode in which to run the generator by default.
     *
     * @see dreisoft.tresos.generator.api.generator.ICodeGenerator#getDefaultMode()
     * @since 2008.b
     */
    @Override
    public String getDefaultMode()
    {
        return MODE_GENERATE;
    }

    /**
     * A NO-OP here.
     */
    @Override
    public void verifySystemModel(CodeGeneratorContext context, APIOperationStatus status)
    {
    }

} // TestGenerator

