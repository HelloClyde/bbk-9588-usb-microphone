# SPDX-License-Identifier: Apache-2.0

"""Generate the BBK 9588 USB microphone menu icon."""

from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image, ImageDraw


SIZE = 256
SCALE = 4

INK = (244, 247, 245, 255)
SURFACE = (15, 27, 34, 255)
TEAL = (48, 211, 194, 255)
CORAL = (255, 92, 92, 255)
GRILLE = (111, 35, 40, 255)


def scaled_box(*values: int) -> tuple[int, ...]:
    return tuple(value * SCALE for value in values)


def generate_icon() -> Image.Image:
    image = Image.new("RGBA", (SIZE * SCALE, SIZE * SCALE), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)

    # Dark circular tile keeps the mark legible on every firmware menu color.
    draw.ellipse(
        scaled_box(18, 18, 238, 238),
        fill=SURFACE,
        outline=TEAL,
        width=10 * SCALE,
    )
    draw.ellipse(
        scaled_box(31, 31, 225, 225),
        outline=(33, 63, 70, 255),
        width=3 * SCALE,
    )

    # Symmetric audio bars stay recognizable in the smallest 54 px resource.
    bars = (
        (47, 108, 59, 148),
        (63, 91, 75, 165),
        (79, 74, 91, 182),
        (165, 74, 177, 182),
        (181, 91, 193, 165),
        (197, 108, 209, 148),
    )
    for bar in bars:
        draw.rounded_rectangle(
            scaled_box(*bar),
            radius=6 * SCALE,
            fill=TEAL,
        )

    # Capsule and grille.
    draw.rounded_rectangle(
        scaled_box(91, 49, 165, 155),
        radius=37 * SCALE,
        fill=INK,
    )
    draw.rounded_rectangle(
        scaled_box(102, 60, 154, 144),
        radius=26 * SCALE,
        fill=CORAL,
    )
    for y, half_width in ((80, 16), (97, 19), (114, 16)):
        draw.rounded_rectangle(
            scaled_box(128 - half_width, y - 4, 128 + half_width, y + 4),
            radius=4 * SCALE,
            fill=GRILLE,
        )

    # Microphone yoke, stem, and a compact USB plug-shaped base.
    draw.arc(
        scaled_box(72, 91, 184, 184),
        start=0,
        end=180,
        fill=INK,
        width=13 * SCALE,
    )
    draw.rounded_rectangle(
        scaled_box(121, 174, 135, 203),
        radius=7 * SCALE,
        fill=INK,
    )
    draw.rounded_rectangle(
        scaled_box(101, 195, 155, 222),
        radius=7 * SCALE,
        fill=INK,
        outline=TEAL,
        width=4 * SCALE,
    )
    draw.rounded_rectangle(
        scaled_box(112, 202, 121, 211),
        radius=2 * SCALE,
        fill=SURFACE,
    )
    draw.rounded_rectangle(
        scaled_box(135, 202, 144, 211),
        radius=2 * SCALE,
        fill=SURFACE,
    )

    return image.resize((SIZE, SIZE), Image.Resampling.LANCZOS)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path(__file__).with_name("9588-usb-mic.png"),
    )
    args = parser.parse_args()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    generate_icon().save(args.output, format="PNG", optimize=True)
    print(f"Generated: {args.output}")


if __name__ == "__main__":
    main()
