/*
 * Copyright (C) EB Automotive
 * All rights reserved
 *
 * TRESOS-4415 ... NGGenerator: file handles not closed for Ant tasks ng.tmplgen and ng.jetgen / ng.jetgen.src
 */

import dreisoft.tresos.generator.api.generator.GeneratorWriter;
import dreisoft.tresos.generator.ng.api.ant.IJavaGenTemplate;
import dreisoft.tresos.generator.ng.api.ant.JavaGenContext;

import java.io.IOException;

public class GeneratorWriterClose implements IJavaGenTemplate
{

  @Override
  public void generate(JavaGenContext context)
  {
    String relVersion = context.gen.getStringVariable("relversion", null);
    if (containsBugfixForTRESOS4415(relVersion))
    {
      return;
    }

    GeneratorWriter writer;
    try
    {
      String file = context.gen.getStringVariable("arg.file", null);
      writer = context.gen.getWriter(file);
      writer.close();
    }
    catch (IOException e)
    {
      e.printStackTrace();
      throw new IllegalStateException("Exception occurred: " + e.getLocalizedMessage());
    }
  }

  private boolean containsBugfixForTRESOS4415(String releaseVersion)
  {
    if (releaseVersion == null)
    {
      throw new IllegalArgumentException("Release version is null!?");
    }

    int indexOfFirstDot = releaseVersion.indexOf('.');
    String majorVersionString = releaseVersion.substring(0, indexOfFirstDot);
    int majorVersion = Integer.parseInt(majorVersionString);
    if (majorVersion < 24)
    {
      return false;
    }

    if (releaseVersion.equals("24.0.0 (X1-DEVELOPMENT-SNAPSHOT)"))
    {
      // TRESOS-4415 is fixed with 24.0.0-X2 (24.0.0-X0 was skipped)
      return false;
    }

    return true;
  }

} // GeneratorWriterClose
